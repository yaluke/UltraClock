CC = g++
OBJS = ClockData.o Input.o InputButton.o LcdUpdater.o LcdTimeUpdater.o LcdLogTimerUpdater.o LcdDoubleLogTimerUpdater.o LcdTimeSmallUpdater.o LcdDateUpdater.o LcdAlarmUpdater.o LcdStoperUpdater.o LcdTimerUpdater.o LcdTempUpdater.o
DEBUG=
CFLAGS = -Wall -std=c++11 -lwiringPi -lwiringPiDev -pthread -c $(DEBUG)
LFLAGS = -Wall -std=c++11 -lwiringPi -lwiringPiDev -pthread $(DEBUG)

test : clock.cpp $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) clock.cpp -o clock

LcdAlarmUpdater.o : LcdAlarmUpdater.h LcdAlarmUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdAlarmUpdater.cpp -o LcdAlarmUpdater.o

LcdStoperUpdater.o : LcdStoperUpdater.h LcdStoperUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdStoperUpdater.cpp -o LcdStoperUpdater.o

LcdTimerUpdater.o : LcdTimerUpdater.h LcdTimerUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTimerUpdater.cpp -o LcdTimerUpdater.o

LcdLogTimerUpdater.o : LcdLogTimerUpdater.h LcdLogTimerUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdLogTimerUpdater.cpp -o LcdLogTimerUpdater.o

LcdDoubleLogTimerUpdater.o: LcdDoubleLogTimerUpdater.h LcdDoubleLogTimerUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdDoubleLogTimerUpdater.cpp -o LcdDoubleLogTimerUpdater.o

LcdTempUpdater.o : LcdTempUpdater.h LcdTempUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTempUpdater.cpp -o LcdTempUpdater.o

LcdDateUpdater.o : LcdDateUpdater.h LcdDateUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdDateUpdater.cpp -o LcdDateUpdater.o

LcdTimeSmallUpdater.o: LcdTimeSmallUpdater.h LcdTimeSmallUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTimeSmallUpdater.cpp -o LcdTimeSmallUpdater.o

LcdTimeUpdater.o : LcdTimeUpdater.h LcdTimeUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTimeUpdater.cpp -o LcdTimeUpdater.o

LcdUpdater.o : LcdUpdater.h LcdUpdater.cpp ClockData.o
	$(CC) $(CFLAGS) LcdUpdater.cpp -o LcdUpdater.o

InputButton.o : InputButton.h InputButton.cpp Input.o
	$(CC) $(CFLAGS) InputButton.cpp -o InputButton.o

Input.o : Input.h Input.cpp ClockData.o
	$(CC) $(CFLAGS) Input.cpp -o Input.o

ClockData.o : ClockData.h ClockData.cpp
	$(CC) $(CFLAGS) ClockData.cpp -o ClockData.o

clean :
	rm -f *.o ._* .D* clock

