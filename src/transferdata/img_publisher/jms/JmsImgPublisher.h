#ifndef GIAPI_JMSIMGPUBLISHER_H
#define GIAPI_JMSIMGPUBLISHER_H

#include "transferdata/img_publisher/ImgPublisher.h"
#include "util/jms/JmsProducer.h"

namespace giapi {
    namespace transferdata {
        namespace img_publisher {
            namespace jms {
                const short TIMEOUT = 2000;
                /**
                 * @class JmsImgPublisher
                 * @brief ActiveMQ-based image publisher using JMS.
                 */
                class JmsImgPublisher : public ImgPublisher, public util::jms::JmsProducer {
                public:
                    /**
                     * @brief Constructor for JmsImgPublisher.
                     * @param detID Detector ID used for topic creation.
                     */
                    JmsImgPublisher(const std::string& detID);

                    /**
                     * @brief Destructor.
                     */
                    virtual ~JmsImgPublisher() = default;

                    static pImgPublisher create(const std::string& detID) noexcept(false);

                    /**
                     * @brief Sends an image to the specified detector.
                     * @param detID Detector ID.
                     * @param imgData Detector image data.
                     * @param blocking If true, blocks until data is received by the client.
                     * @return Status code (0 for success, nonzero for failure).
                     * @throws std::runtime_error If an error occurs.
                     */
                    int sendImage(const std::vector<unsigned char>& binaryData, const bool blocking) noexcept(false) override;
                private:
                    std::string _detID;
                };

            } // namespace jms
        } // namespace img_publisher
    } // namespace transferdata
} // namespace giapi

#endif // GIAPI_JMSIMGPUBLISHER_H
