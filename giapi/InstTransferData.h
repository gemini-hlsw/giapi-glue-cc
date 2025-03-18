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
         * @param detID Detector ID.
         * @param callback Function to handle received data.
         * @return Status code (0 for success, nonzero for failure).
         * @throws std::runtime_error If an error occurs.
         */
        static int receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false);

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