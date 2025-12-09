#include <giapi/InstTransferData.h>
#include "transferdata/ImgTransferDataImpl.h"

namespace giapi {

    void InstTransferData::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&)) {
        ImgTransferDataImpl::receiveImage(detID, callback, false);
    }

    void InstTransferData::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&, uint64_t)) {
        ImgTransferDataImpl::receiveImage(detID, callback, false);
    }

    void InstTransferData::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&), const bool ack) {
        ImgTransferDataImpl::receiveImage(detID, callback, ack);
    }

    void InstTransferData::receiveImage(const std::string& detID, void (*callback)(const std::vector<unsigned char>&, uint64_t), const bool ack){
        return ImgTransferDataImpl::receiveImage(detID, callback, ack);
    }


    int InstTransferData::sendImage(const std::string& detID, const std::vector<unsigned char>& binaryData, bool blocking) noexcept(false) {
        return ImgTransferDataImpl::sendImage(detID, binaryData, blocking);
    }


} // namespace giapi
