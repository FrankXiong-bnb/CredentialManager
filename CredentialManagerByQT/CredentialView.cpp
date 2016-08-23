﻿#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMenu>
#include <QtGui/QContextMenuEvent>

#include "../CredentialManager/bnb_global.h"
#include "../CredentialManager/Credential/Credential.h"

#include "CredentialView.h"

QT_BEGIN_NAMESPACE

CredentialView::CredentialView(bnb::Credential& src, delegate_type* ptrDelegate, QWidget * parent)
    : QWidget(parent)
    , m_ptrDelegate(ptrDelegate)
{
    setAutoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(255, 128, 128));
    setPalette(palette);
    
    _ui.SetupUI(this);

    LayoutView(src.List());

    setFixedSize(sizeHint());

    QObject::connect(_ui.m_actAddPlatform, &QAction::triggered, this, &CredentialView::OnClickedAddPlatform);

    qDebug("w: %d, h: %d", width(), height());
}

void CredentialView::contextMenuEvent(QContextMenuEvent * event)
{
    QMenu menu(this);

    menu.addAction(_ui.m_actAddPlatform);

    menu.exec(event->globalPos());
}

void CredentialView::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void CredentialView::LayoutView(bnb::platform_list & listPlatform)
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->setSpacing(2);
    pLayout->setMargin(1);

    for (auto ptr_platform = listPlatform.Head(); ptr_platform; ptr_platform = ptr_platform->m_Next)
    {
        CredentialItem* pItem = new CredentialItem(ptr_platform->m_Pair.m_Key, ptr_platform->m_Pair.m_Value, this, this);
        pLayout->addWidget(pItem);
    }

    setLayout(pLayout);
}

void CredentialView::OnClickedAddPlatform()
{
    m_ptrDelegate->OnAddPlatform();
}

void CredentialView::ui_type::SetupUI(CredentialView * pView)
{
    m_actAddPlatform = new QAction(pView);

    RetranslateUI(pView);
}

void CredentialView::ui_type::RetranslateUI(CredentialView * pView)
{
    m_actAddPlatform->setText("Add Platform");
}

bool CredentialView::OnAddPlatform()
{
    return false;
}

bool CredentialView::OnAddAccount()
{
    return false;
}

bool CredentialView::OnRemovePlatform(bnb::platform_type * pp)
{
    return false;
}

bool CredentialView::OnRemoveAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    return false;
}

bool CredentialView::OnEditPlatform(bnb::platform_type * pp)
{
    return false;
}

bool CredentialView::OnEditAccount(bnb::platform_type * pp, bnb::account_type * pa)
{
    return false;
}

bool CredentialView::OnViewCredential(bnb::platform_type * pp, bnb::account_type * pa)
{
    return false;
}

QT_END_NAMESPACE
