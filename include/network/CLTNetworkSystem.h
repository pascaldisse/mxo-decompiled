#ifndef _CLT_NETWORK_SYSTEM_H_
#define _CLT_NETWORK_SYSTEM_H_

#include "../CLTBaseClass.h"
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <mutex>
#include <functional>

// Forward declarations
class CLTNetworkConnection;
class CLTPacket;
class CLTEncryptionContext;

/**
 * @brief Type for packet handlers
 */
typedef std::function<bool(CLTPacket*)> PacketHandler;

/**
 * @brief Network communication system
 * 
 * The CLTNetworkSystem manages all network communication between
 * the client and the game servers, including authentication, margin
 * (mission), and game servers.
 */
class CLTNetworkSystem : public CLTBaseClass {
public:
    /**
     * @brief Network protocol types
     */
    enum NetworkProtocol {
        PROTOCOL_TCP,     ///< TCP protocol
        PROTOCOL_UDP,     ///< UDP protocol
        PROTOCOL_HYBRID   ///< Hybrid TCP/UDP protocol
    };
    
    /**
     * @brief Connection status
     */
    enum ConnectionStatus {
        STATUS_DISCONNECTED,    ///< Not connected
        STATUS_CONNECTING,      ///< Connection in progress
        STATUS_CONNECTED,       ///< Connected
        STATUS_AUTHENTICATING,  ///< Authentication in progress
        STATUS_AUTHENTICATED,   ///< Authenticated
        STATUS_ERROR            ///< Error state
    };
    
    /**
     * @brief Server types
     */
    enum ServerType {
        SERVER_AUTH,     ///< Authentication server
        SERVER_MARGIN,   ///< Margin (mission) server
        SERVER_GAME      ///< Game server
    };
    
    /**
     * @brief Error codes
     */
    enum NetworkError {
        ERROR_NONE,                   ///< No error
        ERROR_CONNECTION_FAILED,      ///< Connection attempt failed
        ERROR_CONNECTION_LOST,        ///< Existing connection lost
        ERROR_AUTHENTICATION_FAILED,  ///< Authentication failed
        ERROR_TIMEOUT,                ///< Operation timed out
        ERROR_INVALID_PACKET,         ///< Invalid packet received
        ERROR_ENCRYPTION_FAILED,      ///< Encryption/decryption failed
        ERROR_INTERNAL                ///< Internal error
    };
    
    /**
     * @brief Server connection information
     */
    struct ServerInfo {
        std::string hostname;    ///< Server hostname or IP
        uint16_t port;           ///< Server port
        ServerType type;         ///< Server type
        bool encryption;         ///< Whether encryption is enabled
        std::string publicKey;   ///< Public key for encryption (if applicable)
    };
    
    /**
     * @brief Connection statistics
     */
    struct ConnectionStats {
        uint64_t bytesSent;              ///< Total bytes sent
        uint64_t bytesReceived;          ///< Total bytes received
        uint32_t packetsSent;            ///< Total packets sent
        uint32_t packetsReceived;        ///< Total packets received
        uint32_t packetsLost;            ///< Packets lost
        float latency;                   ///< Current latency in milliseconds
        float averageLatency;            ///< Average latency in milliseconds
        float packetLossRate;            ///< Packet loss rate (0.0 to 1.0)
        uint32_t currentBandwidthIn;     ///< Current incoming bandwidth (bytes/sec)
        uint32_t currentBandwidthOut;    ///< Current outgoing bandwidth (bytes/sec)
        uint32_t peakBandwidthIn;        ///< Peak incoming bandwidth (bytes/sec)
        uint32_t peakBandwidthOut;       ///< Peak outgoing bandwidth (bytes/sec)
    };

public:
    /**
     * @brief Default constructor
     */
    CLTNetworkSystem();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTNetworkSystem();
    
    /**
     * @brief Initialize the network system
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr) override;
    
    /**
     * @brief Clean up resources used by the network system
     */
    virtual void Term() override;
    
    /**
     * @brief Get the class name
     * 
     * @return The class name as a string
     */
    virtual const char* GetClassName() const override;
    
    /**
     * @brief Connect to a server
     * 
     * @param serverInfo Server connection information
     * @return true if connection attempt started successfully, false otherwise
     */
    bool Connect(const ServerInfo& serverInfo);
    
    /**
     * @brief Disconnect from a server
     * 
     * @param serverType Type of server to disconnect from
     * @return true if successful, false otherwise
     */
    bool Disconnect(ServerType serverType);
    
    /**
     * @brief Check if connected to a server
     * 
     * @param serverType Type of server to check
     * @return true if connected, false otherwise
     */
    bool IsConnected(ServerType serverType) const;
    
    /**
     * @brief Get the connection status for a server
     * 
     * @param serverType Type of server
     * @return Connection status
     */
    ConnectionStatus GetConnectionStatus(ServerType serverType) const;
    
    /**
     * @brief Get the last error for a server connection
     * 
     * @param serverType Type of server
     * @return Last error code
     */
    NetworkError GetLastError(ServerType serverType) const;
    
    /**
     * @brief Get the error string for an error code
     * 
     * @param error Error code
     * @return Error description string
     */
    std::string GetErrorString(NetworkError error) const;
    
    /**
     * @brief Authenticate with the authentication server
     * 
     * @param username Username
     * @param password Password
     * @return true if authentication attempt started successfully, false otherwise
     */
    bool Authenticate(const std::string& username, const std::string& password);
    
    /**
     * @brief Send a packet to a server
     * 
     * @param serverType Type of server
     * @param pPacket Packet to send
     * @param reliable Whether the packet must be delivered reliably
     * @return true if the packet was queued for sending, false otherwise
     */
    bool SendPacket(ServerType serverType, CLTPacket* pPacket, bool reliable = true);
    
    /**
     * @brief Register a packet handler for a specific packet type
     * 
     * @param packetType Type of packet to handle
     * @param handler Function to call when packet is received
     */
    void RegisterPacketHandler(uint16_t packetType, PacketHandler handler);
    
    /**
     * @brief Unregister a packet handler
     * 
     * @param packetType Type of packet
     */
    void UnregisterPacketHandler(uint16_t packetType);
    
    /**
     * @brief Process incoming and outgoing packets
     * 
     * @return Number of packets processed
     */
    uint32_t ProcessMessages();
    
    /**
     * @brief Get connection statistics
     * 
     * @param serverType Type of server
     * @param pStats Pointer to receive statistics
     * @return true if statistics were retrieved, false otherwise
     */
    bool GetConnectionStats(ServerType serverType, ConnectionStats* pStats);
    
    /**
     * @brief Set bandwidth limits
     * 
     * @param maxUploadKbps Maximum upload bandwidth in kilobits per second (0 for unlimited)
     * @param maxDownloadKbps Maximum download bandwidth in kilobits per second (0 for unlimited)
     */
    void SetBandwidthLimits(uint32_t maxUploadKbps, uint32_t maxDownloadKbps);
    
    /**
     * @brief Get the current session key
     * 
     * @return Current session key, or empty string if not authenticated
     */
    std::string GetSessionKey() const;
    
    /**
     * @brief Set the packet compression level
     * 
     * @param level Compression level (0-9, 0 = none, 9 = maximum)
     */
    void SetCompressionLevel(uint32_t level);
    
    /**
     * @brief Get the packet compression level
     * 
     * @return Current compression level
     */
    uint32_t GetCompressionLevel() const;
    
    /**
     * @brief Test the connection quality
     * 
     * @param serverType Type of server
     * @param numPings Number of ping packets to send
     * @return Average ping time in milliseconds
     */
    float TestConnection(ServerType serverType, uint32_t numPings = 5);

private:
    // Internal helper methods
    bool ProcessIncomingPackets(ServerType serverType);
    bool ProcessOutgoingPackets(ServerType serverType);
    bool HandlePacket(ServerType serverType, CLTPacket* pPacket);
    bool InitializeEncryption(ServerType serverType, const std::string& publicKey);
    
    // Connection tracking
    std::map<ServerType, CLTNetworkConnection*> m_connections;
    std::map<ServerType, ConnectionStatus> m_connectionStatus;
    std::map<ServerType, NetworkError> m_lastErrors;
    std::map<ServerType, CLTEncryptionContext*> m_encryptionContexts;
    
    // Packet handling
    std::map<uint16_t, PacketHandler> m_packetHandlers;
    std::map<ServerType, std::queue<CLTPacket*>> m_outgoingPackets;
    std::map<ServerType, std::queue<CLTPacket*>> m_incomingPackets;
    std::mutex m_packetMutex;
    
    // Network configuration
    uint32_t m_compressionLevel;
    uint32_t m_maxUploadKbps;
    uint32_t m_maxDownloadKbps;
    std::string m_sessionKey;
    bool m_initialized;
};

#endif // _CLT_NETWORK_SYSTEM_H_