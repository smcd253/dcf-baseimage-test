# How to safe update a DCF package

One of the advantages of using DCF is the safe package update mechanism. It allows developers and administrators to test new packages on the devices before switching to this package and roll back to the old package if necessary.

## Prerequisites 

This tutorial requires that you have created 2 versions of the package key_vault and uploaded it to your blob storage. If you don't have it, please follow the steps in the [DCF Tutorial](DCF_Demo.md).

All commands in this tutorial use Powershell format, where double-quotes [ `"` ] are represented by [ \\\`" ]. If you are following this tutorial using command prompt, please remove the extra [ \` ], keeping only [ \\" ]. For example, when you see "{\\\`"zone\\\`":0}" use "{\\"zone\\":0}" for command prompt. 

## Starting point

For this tutorial, let's start with a device that is using the package "key_vault.1". You can check if your device already contains the key_vault.1 by invoking the "ipc.*.query.1:query" command.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "ipc.*.query.1:query" --mp "{}"

// expected outcome
{
  "payload": {
    "continuation_token": 655615,
    "result": [
      "*ipc.1.query.1",
      "*ipc.1.interface_manager.1",
      "*dm.1.packages.1",
      "*key_vault.1.cipher.1"
    ]
  },
  "status": 200
}
```

If you don't have it, you can install it from your blob storage.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "dm.*.packages.1:install" --mp "{\`"source_type\`":1,\`"package_name\`":\`"https://mystorage.blob.core.windows.net/packages/key_vault.1.bin?sp=r&st=2021-08-12T17:02:13Z&se=2021-09-01T01:02:13Z&sv=2020-08-04&sr=b&sig=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\`"}"

// expected outcome
{
  "payload": {},
  "status": 200
}
```

You can now repeat the query command to make sure that the key_vault.1 was properly installed, and decrypt a secret invoking the command `decrypt` in the interface `cipher.1` exposed by the `key_vault.1` package.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "key_vault.*.cipher.1:decrypt" --mp "{\`"src\`":\`"0ZldfV1pbUhhNXhJHXFAWdkJMQlQgeF1nFEJfQ1AZdXF1ZmF5ZBk\`"}"

// expected outcome
{
  "payload": {
    "dest": "Welcome to the Azure IoT with DCFRTOS!"
  },
  "status": 200
}
```

## Update scenario

The security team realizes that the current implementation of the key_vault uses an algorithm that contains a security issue and shall not be used anymore. They created a new encrypted string to test the new algorithm and sent it to the device.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "key_vault.*.cipher.1:decrypt" --mp "{\`"src\`":\`"1A1xNZx0HEUFZRhAFFUoCDR4eSzR0SE5dTANGEEVIRh0BDEoHBBoDQC10AFQUAkYi\`"}"

// expected outcome
{
  "payload": {
    "description": "AZ_ERROR_NOT_SUPPORTED"
  },
  "status": 405
}
```
Because the current version of the key_vault in the device is old, it does not support this new algorithm and return `AZ_ERROR_NOT_SUPPORTED`.

## Safe update

### Default package

You may asked yourself why we provided `*` as the package version when we invoke the interface commands? The answer is to be able to do the safe update.

As we explained in the [DCF Tutorial](DCF_Demo.md), the `*` means the "DEFAULT" package. So up to now, we always invoke the command in the default package. This should be the standard. All accesses to capability shall always use the default package unless specified otherwise.

But who is the default package? When a new package is installed, if no other package already exposed the interface that this new package will publish, the installed package will be the default one for this interface. You can check that in the "ipc.*.query.1:query", all default packages contains a `*` in front of its name.

To better understand this, let's install a second version of the key_vault that implements both old and new algorithms.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "dm.*.packages.1:install" --mp "{\`"source_type\`":1,\`"package_name\`":\`"https://mokastorage.blob.core.windows.net/packages/key_vault.2.bin?sp=r&st=2021-08-12T17:02:35Z&se=2021-09-01T01:02:35Z&sv=2020-08-04&sr=b&sig=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\`"}"

// expected outcome
{
  "payload": {},
  "status": 200
}
```

Now, querying the ipc will result in 2 key_vault packages, but only the key_vault.1 will have the default mark `*`.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "ipc.*.query.1:query" --mp "{}"

// expected outcome
{
  "payload": {
    "continuation_token": 655615,
    "result": [
      "*ipc.1.query.1",
      "*ipc.1.interface_manager.1",
      "*dm.1.packages.1",
      "*key_vault.1.cipher.1",
      " key_vault.2.cipher.1"
    ]
  },
  "status": 200
}
```

You can see now that both key_vault.1 and key_vault.2 packages are installed, and both expose the interface cipher.1. However, key_vault.1 is still the default for cipher.1. you can check this by sending the `decrypt` command with the new string, which will keep returning `AZ_ERROR_NOT_SUPPORTED`.

### Testing the new package

As we described, key_vault.2 implements both algorithms, so it may replace key_vault.1 even if someone else is still using the old algorithm. But, before putting key_vault.2 in production, let's make sure that it works accordantly. To do this, instead of using the default package, let's specify the package version that we what to use by replacing the wildcard `*` by the version `2`. First let's try with the old string.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "key_vault.2.cipher.1:decrypt" --mp "{\`"src\`":\`"0ZldfV1pbUhhNXhJHXFAWdkJMQlQgeF1nFEJfQ1AZdXF1ZmF5ZBk\`"}"

// expected outcome
{
  "payload": {
    "dest": "Welcome to the Azure IoT with DCFRTOS!"
  },
  "status": 200
}
```

And now, let's try the new string.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "key_vault.2.cipher.1:decrypt" --mp "{\`"src\`":\`"1A1xNZx0HEUFZRhAFFUoCDR4eSzR0SE5dTANGEEVIRh0BDEoHBBoDQC10AFQUAkYi\`"}"

// expected outcome
{
  "payload": {
    "dest": "key_vault can decrypt with the new algorithm :-D"
  },
  "status": 200
}
```

You can create many other tests before approving this new package. Remember that, at this point, all running code that uses the default key_vault will keep running normally using the key_vault.1.

### Put the new package in production

When you are happy with the results from the package, you can exchange the old package with the new one redefining the default package. 

The IPC exposes a second interface called "interface_manager" where you can set the default for an interface, in this case, let's set that key_vault.2 shall be the default package for the interface cipher.1.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "ipc.*.interface_manager.1:set_default" --mp "{\`"interface\`":\`"key_vault.2.cipher.1\`"}"

// expected outcome
{
  "payload": {},
  "status": 200
}
```

If you query the IPC interfaces, you will see that the `*` now marks key_vault.2 as the default.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "ipc.*.query.1:query" --mp "{}"

// expected outcome
{
  "payload": {
    "continuation_token": 655615,
    "result": [
      "*ipc.1.query.1",
      "*ipc.1.interface_manager.1",
      "*dm.1.packages.1",
      " key_vault.1.cipher.1",
      "*key_vault.2.cipher.1"
    ]
  },
  "status": 200
}
```

At this point, all calls to the default key_vault will result in a call to key_vault.2, you can try the default call for the new algorithm that was failing before the update and see that it now succeeds.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "key_vault.*.cipher.1:decrypt" --mp "{\`"src\`":\`"1A1xNZx0HEUFZRhAFFUoCDR4eSzR0SE5dTANGEEVIRh0BDEoHBBoDQC10AFQUAkYi\`"}"

// expected outcome
{
  "payload": {
    "dest": "key_vault can decrypt with the new algorithm :-D"
  },
  "status": 200
}
```

As the final step, you can uninstall key_vault.1 to release the memory.
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "dm.*.packages.1:uninstall" --mp "{\`"package_name\`":\`"key_vault.1\`"}"

// expected outcome
{
  "payload": {},
  "status": 200
}
```
```
az iot hub invoke-device-method -n [name-of-iothub] -d [name-of-device] --mn "ipc.*.query.1:query" --mp "{}"

// expected outcome
{
  "payload": {
    "continuation_token": 655615,
    "result": [
      "*ipc.1.query.1",
      "*ipc.1.interface_manager.1",
      "*dm.1.packages.1",
      "*key_vault.2.cipher.1"
    ]
  },
  "status": 200
}
```

