/**
 ******************************************************************************
 *
 * @file       fieldtreeitem.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup UAVObjectBrowserPlugin UAVObject Browser Plugin
 * @{
 * @brief The UAVObject Browser gadget plugin
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

#ifndef FIELDTREEITEM_H
#define FIELDTREEITEM_H

#include "treeitem.h"
#include <QtCore/QStringList>
#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <qscispinbox/QScienceSpinBox.h>
#include <QComboBox>
#include <limits>
#include <utils/longlongspinbox.h>

#define QINT8MIN std::numeric_limits<qint8>::min()
#define QINT8MAX std::numeric_limits<qint8>::max()
#define QUINTMIN std::numeric_limits<quint8>::min()
#define QUINT8MAX std::numeric_limits<quint8>::max()
#define QINT16MIN std::numeric_limits<qint16>::min()
#define QINT16MAX std::numeric_limits<qint16>::max()
#define QUINT16MAX std::numeric_limits<quint16>::max()
#define QINT32MIN std::numeric_limits<qint32>::min()
#define QINT32MAX std::numeric_limits<qint32>::max()
#define QUINT32MAX std::numeric_limits<quint32>::max()

class FieldTreeItem : public TreeItem
{
    Q_OBJECT
public:
    FieldTreeItem(int index, const QList<QVariant> &data, TreeItem *parent = 0)
        : TreeItem(data, parent)
        , m_index(index)
        , m_defaultValue(true)
    {
    }
    FieldTreeItem(int index, const QVariant &data, TreeItem *parent = 0)
        : TreeItem(data, parent)
        , m_index(index)
        , m_defaultValue(true)
    {
    }
    inline virtual bool isEditable() override { return true; }
    void setIsDefaultValue(bool isDefault) { m_defaultValue = isDefault; }
    virtual bool isDefaultValue() const override { return m_defaultValue; }
    virtual QWidget *createEditor(QWidget *parent) = 0;
    virtual QVariant getEditorValue(QWidget *editor) = 0;
    virtual void setEditorValue(QWidget *editor, QVariant value) = 0;
    virtual void apply() override {}
protected:
    int m_index;
    bool m_defaultValue;
};

class EnumFieldTreeItem : public FieldTreeItem
{
    Q_OBJECT
public:
    EnumFieldTreeItem(UAVObjectField *field, int index, const QList<QVariant> &data,
                      TreeItem *parent = 0)
        : FieldTreeItem(index, data, parent)
        , m_enumOptions(field->getOptions())
        , m_field(field)
    {
    }
    EnumFieldTreeItem(UAVObjectField *field, int index, const QVariant &data, TreeItem *parent = 0)
        : FieldTreeItem(index, data, parent)
        , m_enumOptions(field->getOptions())
        , m_field(field)
    {
    }
    void setData(QVariant value, int column)
    {
        QStringList options = m_field->getOptions();
        QVariant tmpValue = m_field->getValue(m_index);
        int tmpValIndex = options.indexOf(tmpValue.toString());
        setChanged(tmpValIndex != value);
        TreeItem::setData(value, column);
    }
    QString enumOptions(int index)
    {
        if ((index < 0) || (index >= m_enumOptions.length())) {
            return QString("Invalid Value (") + QString().setNum(index) + QString(")");
        }
        return m_enumOptions.at(index);
    }
    void apply()
    {
        int value = data(dataColumn).toInt();
        if (value == -1) {
            qDebug() << "Warning, UAVO browser field is outside range. This should never happen!";
            Q_ASSERT(0);
            return;
        }
        QStringList options = m_field->getOptions();
        m_field->setValue(options[value], m_index);
        setChanged(false);
    }
    void update()
    {
        QStringList options = m_field->getOptions();
        QVariant value = m_field->getValue(m_index);
        int valIndex = options.indexOf(value.toString());
        if (data() != valIndex || changed()) {
            TreeItem::setData(valIndex);
            setHighlight();
        }
        UAVDataObject *obj = qobject_cast<UAVDataObject *>(m_field->getObject());
        if (obj && obj->isSettings())
            setIsDefaultValue(m_field->isDefaultValue(m_index));
    }
    QWidget *createEditor(QWidget *parent)
    {
        QComboBox *editor = new QComboBox(parent);
        editor->setFocusPolicy(Qt::ClickFocus);
        foreach (QString option, m_enumOptions)
            editor->addItem(option);
        return editor;
    }

    QVariant getEditorValue(QWidget *editor)
    {
        QComboBox *comboBox = static_cast<QComboBox *>(editor);
        return comboBox->currentIndex();
    }

    void setEditorValue(QWidget *editor, QVariant value)
    {
        QComboBox *comboBox = static_cast<QComboBox *>(editor);
        comboBox->setCurrentIndex(value.toInt());
    }

private:
    QStringList m_enumOptions;
    UAVObjectField *m_field;
};

class IntFieldTreeItem : public FieldTreeItem
{
    Q_OBJECT
public:
    IntFieldTreeItem(UAVObjectField *field, int index, const QList<QVariant> &data,
                     TreeItem *parent = 0)
        : FieldTreeItem(index, data, parent)
        , m_field(field)
    {
        setMinMaxValues();
    }
    IntFieldTreeItem(UAVObjectField *field, int index, const QVariant &data, TreeItem *parent = 0)
        : FieldTreeItem(index, data, parent)
        , m_field(field)
    {
        setMinMaxValues();
    }

    void setMinMaxValues()
    {
        switch (m_field->getType()) {
        case UAVObjectField::INT8:
            m_minValue = QINT8MIN;
            m_maxValue = QINT8MAX;
            break;
        case UAVObjectField::INT16:
            m_minValue = QINT16MIN;
            m_maxValue = QINT16MAX;
            break;
        case UAVObjectField::INT32:
            m_minValue = QINT32MIN;
            m_maxValue = QINT32MAX;
            break;
        case UAVObjectField::UINT8:
            m_minValue = QUINTMIN;
            m_maxValue = QUINT8MAX;
            break;
        case UAVObjectField::UINT16:
            m_minValue = QUINTMIN;
            m_maxValue = QUINT16MAX;
            break;
        case UAVObjectField::UINT32:
            m_minValue = QUINTMIN;
            m_maxValue = QUINT32MAX;
            break;
        default:
            Q_ASSERT(false);
            break;
        }
    }

    QWidget *createEditor(QWidget *parent)
    {
        switch (m_field->getType()) {
        case UAVObjectField::UINT32:
        {
            LongLongSpinBox *editor = new LongLongSpinBox(parent);
            editor->setRange(m_minValue, m_maxValue);
            editor->setDisplayIntegerBase(m_field->getDisplayIntegerBase());
            editor->setPrefix(m_field->getDisplayPrefix());
            return editor;
        }
        default:
        {
            QSpinBox *editor = new QSpinBox(parent);
            editor->setRange(m_minValue, m_maxValue);
            editor->setDisplayIntegerBase(m_field->getDisplayIntegerBase());
            editor->setPrefix(m_field->getDisplayPrefix());
            return editor;
        }
        }
    }

    QVariant getEditorValue(QWidget *editor)
    {
        if (QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor)) {
            spinBox->interpretText();
            return QVariant(spinBox->value());
        } else if (LongLongSpinBox *spinBox = qobject_cast<LongLongSpinBox *>(editor)) {
            spinBox->interpretText();
            return QVariant(spinBox->value());
        } else {
            Q_ASSERT(false);
            return QVariant();
        }
    }

    void setEditorValue(QWidget *editor, QVariant value)
    {
        switch (m_field->getType()) {
        case UAVObjectField::INT8:
        case UAVObjectField::INT16:
        case UAVObjectField::INT32:
        {
            QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
            spinBox->setValue(value.toInt());
            break;
        }
        case UAVObjectField::UINT8:
        case UAVObjectField::UINT16:
        {
            QSpinBox *spinBox = static_cast<QSpinBox *>(editor);
            spinBox->setValue(value.toUInt());
            break;
        }
        case UAVObjectField::UINT32:
        {
            LongLongSpinBox *spinBox = static_cast<LongLongSpinBox *>(editor);
            spinBox->setValue(value.toUInt());
            break;
        }
        default:
            Q_ASSERT(false);
            break;
        }
    }
    void setData(QVariant value, int column)
    {
        setChanged(m_field->getValue(m_index) != value);
        TreeItem::setData(value, column);
    }
    void apply()
    {
        switch (m_field->getType()) {
        case UAVObjectField::INT8:
        case UAVObjectField::INT16:
        case UAVObjectField::INT32:
            m_field->setValue(data(dataColumn).toInt(), m_index);
            break;
        case UAVObjectField::UINT8:
        case UAVObjectField::UINT16:
        case UAVObjectField::UINT32:
            m_field->setValue(data(dataColumn).toUInt(), m_index);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
        setChanged(false);
    }
    void update()
    {

        QVariant value = m_field->getValue(m_index);
        if (data() != value || changed()) {
            switch (m_field->getType()) {
            case UAVObjectField::INT8:
            case UAVObjectField::INT16:
            case UAVObjectField::INT32:
                TreeItem::setData(value.toInt());
                break;
            case UAVObjectField::UINT8:
            case UAVObjectField::UINT16:
            case UAVObjectField::UINT32:
                TreeItem::setData(value.toUInt());
                break;
            default:
                Q_ASSERT(false);
                break;
            }
            setHighlight();
        }

        UAVDataObject *obj = qobject_cast<UAVDataObject *>(m_field->getObject());
        if (obj && obj->isSettings())
            setIsDefaultValue(m_field->isDefaultValue(m_index));
    }
    QString formattedData()
    {
        QString formatted = m_field->getDisplayPrefix();
        switch (m_field->getType()) {
        case UAVObjectField::INT8:
        case UAVObjectField::INT16:
        case UAVObjectField::INT32:
            formatted += QString::number(TreeItem::data().toInt(), m_field->getDisplayIntegerBase());
            break;
        case UAVObjectField::UINT8:
        case UAVObjectField::UINT16:
        case UAVObjectField::UINT32:
            formatted += QString::number(TreeItem::data().toUInt(), m_field->getDisplayIntegerBase());
            break;
        default:
            Q_ASSERT(false);
            break;
        }
        return formatted;
    }

private:
    UAVObjectField *m_field;
    qint64 m_minValue;
    qint64 m_maxValue;
};

class FloatFieldTreeItem : public FieldTreeItem
{
    Q_OBJECT
public:
    FloatFieldTreeItem(UAVObjectField *field, int index, const QList<QVariant> &data,
                       bool scientific = false, TreeItem *parent = 0)
        : FieldTreeItem(index, data, parent)
        , m_field(field)
        , m_useScientificNotation(scientific)
    {
    }
    FloatFieldTreeItem(UAVObjectField *field, int index, const QVariant &data,
                       bool scientific = false, TreeItem *parent = 0)
        : FieldTreeItem(index, data, parent)
        , m_field(field)
        , m_useScientificNotation(scientific)
    {
    }
    void setData(QVariant value, int column)
    {
        setChanged(m_field->getValue(m_index) != value);
        TreeItem::setData(value, column);
    }
    void apply()
    {
        m_field->setValue(data(dataColumn).toDouble(), m_index);
        setChanged(false);
        UAVDataObject *obj = qobject_cast<UAVDataObject *>(m_field->getObject());
        if (obj && obj->isSettings())
            setIsDefaultValue(m_field->isDefaultValue(m_index));
    }
    void update()
    {
        double value = m_field->getValue(m_index).toDouble();
        if (data() != value || changed()) {
            TreeItem::setData(value);
            setHighlight();
        }
    }

    QWidget *createEditor(QWidget *parent)
    {
        if (m_useScientificNotation) {
            QScienceSpinBox *editor = new QScienceSpinBox(parent);
            editor->setDecimals(6);
            editor->setMinimum(-std::numeric_limits<float>::max());
            editor->setMaximum(std::numeric_limits<float>::max());
            return editor;
        } else {
            QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
            editor->setDecimals(8);
            editor->setMinimum(-std::numeric_limits<float>::max());
            editor->setMaximum(std::numeric_limits<float>::max());
            return editor;
        }
    }

    QVariant getEditorValue(QWidget *editor)
    {
        if (m_useScientificNotation) {
            QScienceSpinBox *spinBox = static_cast<QScienceSpinBox *>(editor);
            spinBox->interpretText();
            return spinBox->value();
        } else {
            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
            spinBox->interpretText();
            return spinBox->value();
        }
    }

    void setEditorValue(QWidget *editor, QVariant value)
    {

        if (m_useScientificNotation) {
            QScienceSpinBox *spinBox = static_cast<QScienceSpinBox *>(editor);
            spinBox->setValue(value.toDouble());
        } else {
            QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox *>(editor);
            spinBox->setValue(value.toDouble());
        }
    }

private:
    UAVObjectField *m_field;
    bool m_useScientificNotation;
};

#endif // FIELDTREEITEM_H
