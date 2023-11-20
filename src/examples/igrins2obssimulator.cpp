/**
 * Example application to evaluate the performance of observations with many
 * items
 */

#include <fstream>
#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/util/concurrent/TimeUnit.h>

#include <giapi/DataUtil.h>
#include <giapi/GiapiErrorHandler.h>
#include <giapi/GiapiUtil.h>
#include <giapi/ServicesUtil.h>
#include <giapi/StatusUtil.h>
#include <src/util/TimeUtil.h>
#include <src/util/PropertiesUtil.h>

#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

using namespace giapi;
using namespace std;

const char *openObservation = "{ \
    \"data_label\": \"%s\", \
    \"keywords\": [] \
  }";

const char *closeObservation = "{ \
    \"data_label\": \"%s\" \
  }";

const char *storeKeyword = "{\
    \"data_label\": \"%s\", \
    \"keywords\": [ \
      {\
        \"keyword\": \"GEMPRGID\", \
        \"value_type\": \"STRING\", \
        \"value\": \"1\" \
      },\
      {\
        \"keyword\": \"AMEND\", \
        \"value_type\": \"DOUBLE\", \
        \"value\": \"1000.0\" \
      } \
    ] \
  }";

string copyDataFile (string dataLocation, std::string dataLabel) {
  string newLocation (dataLocation);
  newLocation.append ("/");
  newLocation.append (dataLabel);

  cout << "TempFile : " << newLocation << endl;

  ifstream in ("N20231009S0059.fits", ifstream::binary);
  ofstream out (newLocation.c_str (), ifstream::binary);
  out << in.rdbuf ();

  return newLocation;
}

void postGDSRequest (std::string dataLabel, std::string route, const char *xml) {
  try {
      curlpp::Cleanup cleaner;
      curlpp::Easy request;

      std::list<std::string> header;

      header.push_back ("Content-Type: application/json");

      char *xmlRequest = new char[dataLabel.size () + strlen (xml)];
      sprintf (xmlRequest, xml, dataLabel.c_str ());
      std::istringstream instream (xmlRequest);
      int size = instream.str ().size ();

      char buf[50];
      sprintf (buf, "Content-Length: %d", size);
      header.push_back (buf);
      using namespace curlpp::Options;
      std::string hostname = giapi::util::PropertiesUtil::Instance().getProperty("gmp.hostname");

      request.setOpt (new Url ("http://" + hostname + ":8088/gds-seqexec/" + route));
      request.setOpt (new HttpHeader (header));
      request.setOpt (new Post (true));
      request.setOpt (new InfileSize (size));
      request.setOpt (new ReadStream (&instream));

      request.perform ();
  } catch (curlpp::LogicError &e) {
      std::cout << e.what () << std::endl;
      exit(1);
  } catch (curlpp::RuntimeError &e) {
      std::cout << e.what () << std::endl;
      exit(1);
  }
}

void postEvent (data::ObservationEvent eventType, int delay, std::string dataLabel) {
  cout << "POST " << eventType << endl;
  if (DataUtil::postObservationEvent (eventType, dataLabel) == status::ERROR) {
    cout << "ERROR posting " << eventType << endl;
  }

  decaf::util::concurrent::TimeUnit::MILLISECONDS.sleep (delay);
}

void terminate (int signal) {
  std::cout << "Exiting... " << std::endl;
  exit (1);
}

int main (int argc, char **argv) {
  try {
      std::cout << "Starting GPI Simulator Test" << std::endl;

      signal (SIGABRT, terminate);
      signal (SIGTERM, terminate);
      signal (SIGINT, terminate);

      time_t seconds;
      seconds = time (NULL);

      std::stringstream ss;
      ss << "S" << seconds << ".fits";
      cout << "Simulating datalabel " << ss.str () << endl;
      std::string dataLabel = ss.str ();

      struct timeval start, end;
      gettimeofday (&start, NULL);
      postGDSRequest(dataLabel, "open-observation", openObservation);
      postGDSRequest(dataLabel, "keywords", storeKeyword);
      gettimeofday (&end, NULL);
      double dif = (end.tv_sec - start.tv_sec) * 1000.0
                   + (end.tv_usec - start.tv_usec) / 1000.0;

      cout << endl << "time to post gds requests:" << dif << " [ms]" << endl;

      string dataLocation
          = ServicesUtil::getProperty ("DHS_SCIENCE_DATA_PATH", 1000);

      string newLocation = copyDataFile (dataLocation, dataLabel);
      std::cout << "Copied file to " << newLocation << std::endl;

      postEvent (data::OBS_PREP, 400, dataLabel);
      postEvent (data::OBS_START_ACQ, 3000, dataLabel);
      postEvent (data::OBS_END_ACQ, 200, dataLabel);
      postEvent (data::OBS_START_READOUT, 1, dataLabel);
      postEvent (data::OBS_END_READOUT, 1, dataLabel);
      postEvent (data::OBS_START_DSET_WRITE, 200, dataLabel);
      postEvent (data::OBS_END_DSET_WRITE, 1, dataLabel);
      postGDSRequest(dataLabel, "close-observation", closeObservation);
  } catch (GmpException &e) {
    std::cerr << e.getMessage () << ". Is the GMP up?" << std::endl;
  }
  return 0;
}
