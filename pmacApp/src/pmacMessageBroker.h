/*
 * pmacMessageBroker.h
 *
 *  Created on: 4 Feb 2016
 *      Author: gnx91527
 */

#ifndef PMACAPP_SRC_PMACMESSAGEBROKER_H_
#define PMACAPP_SRC_PMACMESSAGEBROKER_H_

#include "asynDriver.h"
#include "asynOctet.h"
#include "asynOctetSyncIO.h"
#include "epicsTime.h"
#include "pmacDebugger.h"
#include "pmacCommandStore.h"
#include "pmacCallbackStore.h"
#include "pmacCallbackInterface.h"
#include <string.h>

class pmacMessageBroker : public pmacDebugger
{
public:
  static const epicsInt32 PMAC_SLOW_READ   = 0;
  static const epicsInt32 PMAC_MEDIUM_READ = 1;
  static const epicsInt32 PMAC_FAST_READ   = 2;

  pmacMessageBroker(asynUser *pasynUser);
  virtual ~pmacMessageBroker();
  asynStatus connect(const char *port, int addr);
  asynStatus getConnectedStatus(int *connected);
  asynStatus immediateWriteRead(const char *command, char *response);
  asynStatus addReadVariable(int type, const char *variable);
  asynStatus updateVariables(int type);
  asynStatus registerForUpdates(pmacCallbackInterface *cbPtr, int type);
  double readUpdateTime();
  asynStatus readStatistics(int *noOfMsgs,
                            int *totalBytesWritten,
                            int *totalBytesRead,
                            int *totalMsgTime,
                            int *lastMsgBytesWritten,
                            int *lastMsgBytesRead,
                            int *lastMsgTime);
  asynStatus readStoreSize(int type, int *size);
  asynStatus report(int type);

private:
  asynStatus lowLevelPortConnect(const char *port, int addr, asynUser **ppasynUser, char *inputEos, char *outputEos);
  asynStatus lowLevelWriteRead(const char *command, char *response);

  // Mutex required for locking across threads
  epicsMutex mutex_;

  asynUser* ownerAsynUser_;
  asynUser* lowLevelPortUser_;

  // Command storage
  pmacCommandStore slowStore_;
  pmacCommandStore mediumStore_;
  pmacCommandStore fastStore_;

  // Callback storage
  pmacCallbackStore *slowCallbacks_;
  pmacCallbackStore *mediumCallbacks_;
  pmacCallbackStore *fastCallbacks_;

  // Recording of statistics
  int noOfMessages_;
  int totalBytesWritten_;
  int totalBytesRead_;
  int totalMsgTime_;
  int lastMsgBytesWritten_;
  int lastMsgBytesRead_;
  int lastMsgTime_;
  epicsTimeStamp writeTime_;
  epicsTimeStamp startTime_;
  epicsTimeStamp currentTime_;

  // Update time in ms
  double updateTime_;

  static const epicsUInt32 PMAC_MAXBUF_;
  static const epicsFloat64 PMAC_TIMEOUT_;
};

#endif /* PMACAPP_SRC_PMACMESSAGEBROKER_H_ */
