﻿#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "credential_qt_utils.h"

#include "Widget/PasswordInput.h"

QT_BEGIN_NAMESPACE

PasswordInput::PasswordInput(QWidget * parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint)
{
    _ui.SetupUI(this);

    QObject::connect(_ui.m_btnOK, &QPushButton::clicked, this, &QDialog::accept);
    QObject::connect(_ui.m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

QString PasswordInput::GetPassword() const
{
    return _ui.m_editPassword->text();
}

void PasswordInput::ui_type::SetupUI(PasswordInput * pDlg)
{
    ui_utils::SetBackgroundColor(pDlg, Qt::white);

    _labPassword = ui_utils::MakeStaticLabel(pDlg, ui_utils::lab_default_w, ui_utils::lab_default_h, QColor(96, 96, 255));

    m_editPassword = new QLineEdit(pDlg);
    m_editPassword->setEchoMode(QLineEdit::Password);
    m_editPassword->setFixedSize(ui_utils::edit_password_w, ui_utils::edit_password_h);

    m_btnOK = ui_utils::MakeButton(pDlg);
    m_btnCancel = ui_utils::MakeButton(pDlg);

    QHBoxLayout* phLayout1 = new QHBoxLayout;
    phLayout1->setMargin(0);
    phLayout1->setSpacing(0);
    phLayout1->addWidget(_labPassword);
    phLayout1->addWidget(m_editPassword);

    QHBoxLayout* phLayout2 = new QHBoxLayout;
    phLayout2->setContentsMargins(0, 12, 0, 12);
    phLayout2->setSpacing(0);
    phLayout2->addStretch(1);
    phLayout2->addWidget(m_btnOK);
    phLayout2->addStretch(1);
    phLayout2->addWidget(m_btnCancel);
    phLayout2->addStretch(1);

    QVBoxLayout* pvMainLayout = new QVBoxLayout;
    pvMainLayout->setContentsMargins(4, 8, 4, 4);
    pvMainLayout->setSpacing(4);
    pvMainLayout->addLayout(phLayout1);
    pvMainLayout->addLayout(phLayout2);

    pDlg->setLayout(pvMainLayout);
    pDlg->setFixedSize(pDlg->sizeHint());

    RetranslateUI(pDlg);
}

void PasswordInput::ui_type::RetranslateUI(PasswordInput * pDlg)
{
    _labPassword->setText("Password: ");

    m_btnOK->setText("OK");
    m_btnCancel->setText("Cancel");
}

QT_END_NAMESPACE
