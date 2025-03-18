#ifndef GIAPI_IMGPUBLISHER_H
#define GIAPI_IMGPUBLISHER_H

#include <string>
#include <tr1/memory>
#include <vector>

namespace giapi {
    namespace transferdata {
        namespace img_publisher{
            class ImgPublisher {
            public:
                virtual int sendImage(const std::vector<unsigned char>& binaryData, const bool blocking) noexcept(false) = 0;
                //pImgPublisher create(const std::string& detID) =0;
            };
        typedef std::tr1::shared_ptr<ImgPublisher> pImgPublisher;
        }
    }
} // namespace giapi

#endif // GIAPI_IMGPUBLISHER_H
