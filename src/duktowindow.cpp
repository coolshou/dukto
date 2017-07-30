/* DUKTO - A simple, fast and multi-platform file transfer tool for LAN users
 * Copyright (C) 2011 Emanuele Colombo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "duktowindow.h"
#include "guibehind.h"
#include "platform.h"
#include "settings.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QMimeData>

#if QT_VERSION < QT_VERSION_CHECK (5, 0, 0)
DuktoWindow::DuktoWindow(QWidget *parent) :
#else
DuktoWindow::DuktoWindow(QWindow *parent) :
#endif
    QmlApplicationViewer(parent), mGuiBehind(NULL)
{
    // Configure window
#if QT_VERSION < QT_VERSION_CHECK (5, 0, 0)
    setAcceptDrops(true);
    setWindowTitle("Dukto");
	setWindowIcon(QIcon(":/dukto.png"));

#ifndef Q_OS_S60
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setMaximumSize(350, 5000);
    setMinimumSize(350, 500);
#endif
#else
    qDebug() <<"TODO:  Configure window";
#endif
	//TODO: set ScreenOrientation by accelerometers to detect if it's orientation was changed
	//QmlApplicationViewer::ScreenOrientationLockLandscape
    setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
#if defined(ANDROID)
    KeepAwakeHelper helper;
    // screen and CPU will stay awake during this section
    // lock will be released when helper object goes out of scope
#endif
    // Taskbar integration with Win7
    mWin7.init(this->winId());

}

#ifdef Q_OS_WIN
bool DuktoWindow::winEvent(MSG * message, long * result)
{
    return mWin7.winEvent(message, result);
}
#endif

void DuktoWindow::setGuiBehindReference(GuiBehind* ref)
{
    mGuiBehind = ref;
}

void DuktoWindow::dragEnterEvent(QDragEnterEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls() && mGuiBehind->canAcceptDrop())
        event->acceptProposedAction();
}

void DuktoWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void DuktoWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void DuktoWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (!mimeData->hasUrls()) return;

    QStringList files;
    const QList<QUrl> urlList = mimeData->urls();
    foreach (const QUrl &url, urlList)
        files.append(url.toLocalFile());

       event->acceptProposedAction();
    mGuiBehind->sendDroppedFiles(&files);
}

void DuktoWindow::closeEvent(QCloseEvent *event)
{
    //this never trigger on Qt5.9 QQuickView? why?
    //qDebug() << "TODO:closeEvent saveGeometry:" << frameGeometry();
    #if QT_VERSION < QT_VERSION_CHECK (5, 0, 0)
    mGuiBehind->settings()->saveWindowGeometry(saveGeometry());
    #else
    mGuiBehind->settings()->saveWindowGeometry(frameGeometry());
    #endif
    if (mGuiBehind->isTrayIconVisible()) {
        hide();
        event->ignore();
    } else {
        mGuiBehind->close();
    }
}
bool DuktoWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Close) {
#if QT_VERSION < QT_VERSION_CHECK (5, 0, 0)
        mGuiBehind->settings()->saveWindowGeometry(saveGeometry());
#else
        mGuiBehind->settings()->saveWindowGeometry(frameGeometry());
#endif
        if (mGuiBehind->isTrayIconVisible()) {
            hide();
            event->ignore();
        } else {
            show(); //must show() then it can be close
            event->accept();
        }
    }
    return QQuickView::event(event);
}

void DuktoWindow::showEvent(QShowEvent *event)
{
#if QT_VERSION < QT_VERSION_CHECK (5, 0, 0)
    restoreGeometry(mGuiBehind->settings()->windowGeometry());
#else
    setGeometry(QRect(mGuiBehind->settings()->windowRect()));
#endif
    Q_UNUSED(event);
}

void DuktoWindow::exit(){
    mGuiBehind->setTrayIconVisible(false);
    close();
}
