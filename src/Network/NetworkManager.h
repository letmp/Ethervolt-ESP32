#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <Arduino.h>
#include <TinyMqtt.h>   // https://github.com/hsaturn/TinyMqtt
#include <ESPmDNS.h>
#include <ETH.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <Persistence/PersistenceManager.h>
#include <Common/Globals.h>

#define ETH_ADDR        0 	// I²C-address of Ethernet PHY (0 or 1 for LAN8720, 31 for TLK110)
#define ETH_POWER_PIN   -1 	// Pin# of the enable signal for the external crystal oscillator (-1 to disable for internal APLL source)
#define ETH_MDC_PIN     23 	// Pin# of the I²C clock signal for the Ethernet PHY
#define ETH_MDIO_PIN    18	// Pin# of the I²C IO signal for the Ethernet PHY
#define ETH_TYPE        ETH_PHY_LAN8720
#ifdef ETH_CLK_MODE 
#undef ETH_CLK_MODE
#endif
#define ETH_CLK_MODE   	ETH_CLOCK_GPIO17_OUT // ETH_CLOCK_GPIO0_IN

class NetworkManager {
	
	private:
		static PersistenceManager mPersistenceManager;

		String mWifiSSID;
		String mWifiPassword;
		IPAddress mIpAddressWifi;
		IPAddress mIpAddressWifiGateway;
		IPAddress mIpAddressWifiSubnet;
		IPAddress mIpAddressEth;
		String mMacAddressWifi;
		
		AsyncWebServer mAsyncWebServer;
		
		static void writeParams(AsyncWebServerRequest * request);
		static void writeParameterToSPIFFS(AsyncWebParameter* p, String parameter);

		MqttBroker mBroker;
		IPAddress mBrokerIp;
		uint16_t mBrokerPort;
		MqttClient mClientState;
		MqttClient mClientDataTransfer;
		std::string mTopicState="states/update";

		static void stateUpdate(const MqttClient*, const Topic& topic, const char* payload, size_t );

	public:
		bool isMaster = false;
		
		NetworkManager();
		
		bool loadWifiConfig();
		bool loadEthConfig();

		bool initWifiAP();
		bool initWifiSTA();
		void initETH();

		void startWebServerAP();
		void startWebServerSTA();
		
		void startBroker();
		void initMdns();
		void initClients();
		
		void loop();
};

#endif