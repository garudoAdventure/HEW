#define CONIOEX
#include "conioex.h"
#include "mic.h"
#include "endpointvolume.h"
#include "mmdeviceapi.h"
#include "iostream"

IMMDeviceEnumerator* pEnum = NULL;
IMMDeviceCollection* pDeviceCollection = NULL;
IMMDevice* pDevice = NULL;
IAudioMeterInformation* pMeterInfo = NULL;

float peak;

int peakTime;

void micInit() {
  peak = 0.0f;
  peakTime = 0;

  // 記得要先把マイク設定介面打開

  CoInitializeEx(0, COINIT_APARTMENTTHREADED);
  CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
  pEnum->EnumAudioEndpoints(eAll, DEVICE_STATE_ACTIVE | DEVICE_STATE_UNPLUGGED, &pDeviceCollection);  
  
  // pEnum->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);   // For Default Mic
  pDeviceCollection->Item(11, &pDevice); // For USB Mic
  
  pDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&pMeterInfo);
  
  // Check mic device name
 // for (int i = 0; i < 20; i++) {
	//getMicName(i);
 // }
}

void micUpdate() {
  pMeterInfo->GetPeakValue(&peak);
  if (peak * 100.0f > 90.0f) {
	peakTime++;
  }
}

void micDestroy() {

}

float getMicPeak() {
  return peak * 100.0f;
}

int getPeakTime() {
  return peakTime;
}

void getMicName(int count) {
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