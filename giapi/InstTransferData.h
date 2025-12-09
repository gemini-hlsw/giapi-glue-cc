#ifndef GIAPI_INSTTRANSFERDATA_H
#define GIAPI_INSTTRANSFERDATA_H

#include <string>
#include <vector>

namespace giapi {

    /**
     * @class InstTransferData
     * @brief Handles the transmission and reception of instrument images over ActiveMQ.
     */
    class InstTransferData {
    public:
        /**
         * @brief Receives an image asynchronously using a callback.
         *
         * @param detID Detector topic identifier.
         * @param callback Non-owning function pointer invoked with the received data bytes.
         * @return Status code (0 for success, nonzero for failure).
         * @throws std::runtime_error If an error occurs.
         */
        static int receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false);


        /**
         * @brief Receives an image asynchronously using a callback that also receives a timestamp.
         *
         * @param detID Detector topic identifier.
         * @param callback Non-owning function pointer invoked with the received bytes and a timestamp.
         *                 Timestamp semantics: microseconds representing message time or delay.
         * @return Status code (0 for success, nonzero for failure).
         * @throws std::runtime_error If an error occurs.
         */
        static int receiveImage(const std::string& detID, 
                                void (*callback)(const std::vector<unsigned char>&, uint64_t)
                               ) noexcept(false);
        

        /**
         * @brief Receives an image asynchronously using a callback.
         *
         * @param detID Detector topic identifier.
         * @param callback Non-owning function pointer invoked with the received data data bytes.
         * @param ack When true, the implementation will publish lightweight ACKs according to protocol.
         * @return Status code (0 for success, nonzero for failure).
         * @throws std::runtime_error If an error occurs.
         */
        static int receiveImage(const std::string& detID, 
                                void (*callback)(const std::vector<unsigned char>&), 
                                const bool ack) noexcept(false);

        /**
         * @brief Receives an image asynchronously using a callback that also receives a timestamp.
         *
         * @param detID Detector topic identifier.
         * @param callback Non-owning function pointer invoked with the received bytes and a timestamp.
         *                 Timestamp semantics: microseconds representing message time delay.
         * @param ack When true, the implementation will publish lightweight ACKs according to protocol.
         * @return Status code (0 for success, nonzero for failure).
         * @throws std::runtime_error If an error occurs.
         */

        static int receiveImage(const std::string& detID, 
                                void (*callback)(const std::vector<unsigned char>&, uint64_t),
                                const bool ack) noexcept(false);


        /**
         * @brief Sends an image to the specified detector.
         *
         * @param detID Detector ID.
         * @param binaryData Binary vector containing FITS data.
         * @param blocking If true, blocks until data is received by the client.
         * @return Status code (0 for success, nonzero for failure).
         * @throws std::runtime_error If an error occurs.
         */
        static int sendImage(const std::string& detID, const std::vector<unsigned char>& binaryData, bool blocking) noexcept(false);
    };

} // namespace giapi

#endif // GIAPI_INSTTRANSFERDATA_H
