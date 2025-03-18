#ifndef GIAPI_IMGSUBSCRIBER_H
#define GIAPI_IMGSUBSCRIBER_H

#include <string>
#include <tr1/memory>
#include <vector>

namespace giapi {
    namespace transferdata {
        namespace img_subscriber{
            class ImgSubscriber {
            public:
                virtual int receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) noexcept(false) = 0;
                virtual void stopReceive() noexcept(false) = 0;
            };
        typedef std::tr1::shared_ptr<ImgSubscriber> pImgSubscriber;
        }
    }
} // namespace giapi

#endif // GIAPI_IMGSUBSCRIBER_H
