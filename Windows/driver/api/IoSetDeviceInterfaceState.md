IoSetDeviceInterfaceState function (wdm.h)
04/30/2018
2 minutes to read
The IoSetDeviceInterfaceState routine enables or disables an instance of a previously registered device interface class.

Syntax
C++

Copy
NTSTATUS IoSetDeviceInterfaceState(
  PUNICODE_STRING SymbolicLinkName,
  BOOLEAN         Enable
);
Parameters
SymbolicLinkName

[in] Pointer to a string that identifies the device interface instance that is being enabled or disabled. This string was obtained from a previous call to IoRegisterDeviceInterface or IoGetDeviceInterfaces.

Enable

[in] TRUE indicates that the device interface is being enabled. FALSE indicates that the device interface is being disabled.

Return value
IoSetDeviceInterfaceState returns STATUS_SUCCESS if the call was successful. This routine returns an informational status of STATUS_OBJECT_NAME_EXISTS if the caller requested to enable a device interface that was already enabled. Possible error return values are described following.

RETURN VALUE
Return code	Description
STATUS_OBJECT_NAME_NOT_FOUND
The caller tried to disable a device interface that was not enabled.
Remarks
IoSetDeviceInterfaceState enables an instance of a registered device interface for use by applications and other system components. The interface class must have been previously registered with IoRegisterDeviceInterface or from user mode.

Applications and other system components can open only interfaces that are enabled.

A function or a filter driver typically calls this routine with Enable set to TRUE after it successfully starts a device in response to an IRP_MN_START_DEVICE IRP. Such a driver should disable the device interface instance (that is, call IoSetDeviceInterfaceState and set Enable to FALSE) when it removes the device in response to an IRP_MN_REMOVE_DEVICE IRP or an IRP_MN_SURPRISE_REMOVAL IRP. If a driver does not disable a device interface when it processes these removal IRPs, the driver should not subsequently attempt to do so because the PnP manager will disable the interface when the PnP manager removes the device.

If a device is removed suddenly (for example, by a surprise removal) but still has a valid device interface instance, a problem will occur if the device is reattached. This problem occurs when the PnP manager enumerates the newly attached device and enables a device interface instance, which will exist at the same registry path as the existing device interface instance.

Note that if the driver calls IoSetDeviceInterfaceState to disable a device interface instance in response to an IRP_MN_SURPRISE_REMOVAL IRP, the driver must not attempt to disable the same device interface instance in response to an IRP_MN_REMOVE_DEVICE IRP.

If a call to IoSetDeviceInterfaceState successfully exposes a device interface instance, the system notifies any components that registered for PnP notification of a device class change. Similarly, if a call to this routine disables an existing device interface instance, the system sends appropriate notifications.

The PnP manager does not send notification of interface instance arrivals until the IRP_MN_START_DEVICE IRP completes, indicating that all the drivers for the device have completed their start operations. In addition, the PnP manager fails create requests for the device until the IRP_MN_START_DEVICE IRP completes.

Callers of IoSetDeviceInterfaceState must be running at IRQL = PASSIVE_LEVEL in the context of a system thread.