#include "audio.h"
#include "endpointvolume.h"
#include "mmdeviceapi.h"

IMMDeviceEnumerator* pEnum = NULL;
IMMDeviceCollection* pDeviceCollection = NULL;
IMMDevice* pDevice = NULL;
IAudioMeterInformation* pMeterInfo = NULL;

float peak;

void audioInit() {
  peak = 0.0f;

  // 記得要先把マイク設定介面打開

  CoInitializeEx(0, COINIT_MULTITHREADED);
  CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
  pEnum->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE | DEVICE_STATE_UNPLUGGED, &pDeviceCollection);  
  pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
  // pDeviceCollection->Item(6, &pDevice);
  pDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&pMeterInfo);
}

void audioUpdate() {
  pMeterInfo->GetPeakValue(&peak);
}

void audioRender() {

}

void audioDestroy() {

}

float getPeak() {
  return peak * 100.0f;
}

void getAudioName(int count) {
  pDeviceCollection->Item(count, &pDevice);
  IPropertyStore* propertyStore;
  pDevice->OpenPropertyStore(STGM_READ, &propertyStore);
  static PROPERTYKEY key;
  GUID IDevice_FriendlyName = { 0xa45c254e, 0xdf1c, 0x4efd, { 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0 } };
  key.pid = 14;
  key.fmtid = IDevice_FriendlyName;
  PROPVARIANT varName;
  PropVariantInit(&varName);
  propertyStore->GetValue(key, &varName);
}