/**
 ******************************************************************************
 *
 * @file       importsummary.h
 * @author     (C) 2011 The OpenPilot Team, http://www.openpilot.org
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup UAVSettingsImportExport UAVSettings Import/Export Plugin
 * @{
 * @brief UAVSettings Import/Export Plugin
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>
 */
#ifndef IMPORTSUMMARY_H
#define IMPORTSUMMARY_H

#include <QDialog>
#include <QCheckBox>
#include <QDesktopServices>
#include <QUrl>
#include "ui_importsummarydialog.h"
#include "uavobjects/uavdataobject.h"
#include "uavobjects/uavobjectmanager.h"
#include "extensionsystem/pluginmanager.h"
#include "uavobjectutil/uavobjectutilmanager.h"

namespace Ui {
class ImportSummaryDialog;
}

class ImportSummaryDialog : public QDialog
{
    Q_OBJECT

public:
    ImportSummaryDialog(QWidget *parent = 0, bool quiet = false);
    ~ImportSummaryDialog();
    void addLine(QString objectName, QString text, bool status);
    void setUAVOSettings(UAVObjectManager *obj);
    int numLines() const;

protected:
    void showEvent(QShowEvent *event);
    void changeEvent(QEvent *e);

private:
    Ui::ImportSummaryDialog *ui;
    UAVObjectManager *importedObjects;
    bool quiet;

public slots:
    void updateCompletion();

private slots:
    void doTheApplySaving();
    void setCheckedState(int state);
    void openHelp();
};

#endif // IMPORTSUMMARY_H
