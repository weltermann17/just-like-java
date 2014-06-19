#!/usr/bin/ksh

export JMS_PROVIDER_HOST=MNDEMUCENOVT2
export JMS_PROVIDER_PORT=14014
export JMS_MINIMUM_POOLSIZE=32
export JMS_MAXIMUM_POOLSIZE=256
export JMS_LOGGING_LEVEL=FINER

JMSProvider
rc=$?
echo "rc="$rc
exit $rc

#eof
