#include <QApplication>
#include <QLockFile>
#include <cstdio>

#include <primarywidget.h>

static void check_lock_file(QLockFile &lkfile);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Gadget");

    QLockFile lkfile("/tmp/.gadget.lock");
    check_lock_file(lkfile);

    PrimaryWidget p;
    p.show();

    return a.exec();
}

static void check_lock_file(QLockFile &lkfile)
{
    if(lkfile.tryLock() == false)
    {
        qint64 lk_pid;
        QString hostname;
        QString appname;
        lkfile.getLockInfo(&lk_pid, &hostname, &appname);
        fprintf(stderr, "gadget is already running. pid: %lld\n", lk_pid);
        exit(1);
    }
}
