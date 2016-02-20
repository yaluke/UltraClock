CC = g++
OBJS = ClockData.o Input.o InputButton.o LcdUpdater.o LcdTimeUpdater.o LcdDateUpdater.o LcdAlarmUpdater.o LcdStoperUpdater.o LcdTimerUpdater.o LcdTemp1Updater.o LcdTemp2Updater.o LcdTemp3Updater.o
DEBUG = -g
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

LcdTemp1Updater.o : LcdTemp1Updater.h LcdTemp1Updater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTemp1Updater.cpp -o LcdTemp1Updater.o

LcdTemp2Updater.o : LcdTemp2Updater.h LcdTemp2Updater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTemp2Updater.cpp -o LcdTemp2Updater.o

LcdTemp3Updater.o : LcdTemp3Updater.h LcdTemp3Updater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdTemp3Updater.cpp -o LcdTemp3Updater.o

LcdDateUpdater.o : LcdDateUpdater.h LcdDateUpdater.cpp LcdUpdater.o
	$(CC) $(CFLAGS) LcdDateUpdater.cpp -o LcdDateUpdater.o

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

