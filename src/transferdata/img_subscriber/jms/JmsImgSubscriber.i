template<typename CallbackType>
int JmsImgSubscriber::_checkReceiving(CallbackType callback) noexcept(false) {
    if (_isReceiving) {
        LOG4CXX_WARN(logger, "Client stopped of receiving FITS data for detectorID " << _detID << " before starting a new reception.");
        return 1;
    }

    if (callback == nullptr) {
        LOG4CXX_ERROR(logger, "Callback function is null for detectorID " << _detID << ". Please, provide a valid callback.");
        throw std::runtime_error("No callback function provided. Cannot start receiving FITS data.");
    }

    return 0;
}
