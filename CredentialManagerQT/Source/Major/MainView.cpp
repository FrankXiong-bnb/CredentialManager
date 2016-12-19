#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QSplitter>

#include "Credential/Credential.h"

#include "credential_qt_string.h"
#include "credential_qt_utils.h"
#include "credential_qt_delegate.h"

#include "Widget/AboutDialog.h"
#include "Widget/HintDialog.h"
#include "Widget/PasswordInput.h"
#include "Widget/NewDialog.h"
#include "Widget/EditDialog.h"

#include "Major/ToolBar.h"
#include "Major/StackView.h"
#include "Major/TreeView.h"
#include "Major/MainView.h"

bnb::Credential& g_Credential()
{
    static bnb::Credential _just_a_credential_;
    return (_just_a_credential_);
}

QT_BEGIN_NAMESPACE

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setWindowOpacity(0.96);

    _ui.SetupUI(this);
}

bool MainView::SaveCredential()
{
    return g_Credential().Save(m_strFile.toStdString().c_str());
}

void MainView::AddCredential()
{
    _ui.m_treeView->AddCredential(g_Credential());

    _ui.m_viewStack->AddCredential(g_Credential());

    _ui.m_viewToolBar->UpdatePath(m_strFile);
}

void MainView::ClearCredential()
{
    _ui.m_treeView->ClearCredential();
    _ui.m_viewStack->ClearCredential();
}

void MainView::OnClickedNew()
{
    NewDialog dlg(this);

    if (QDialog::Accepted == dlg.exec())
    {
        m_strFile = dlg.GetFileName();

        g_Credential().Clear();
        g_Credential().SetWord(From_QString(dlg.GetPassword()));
        g_Credential().SetUser(From_QString(dlg.GetUserName()));
        g_Credential().SetComment(From_QString(dlg.GetComment()));
        
        SaveCredential();
        ClearCredential();
        AddCredential();
    }
}

void MainView::OnClickedOpen()
{
    QString strFile = QFileDialog::getOpenFileName(
        this, "Please select a credential file", ".", "credential file(*.credential)");

    if (!strFile.isEmpty())
    {
        bnb::memory_type _xml;

        if (!bnb::Credential::CheckFile(strFile.toStdString().c_str(), &_xml))
        {
            HintDialog(hint_type::ht_error, "You selected file invalid !", this).exec();
            return;
        }

        PasswordInput dlg(this);
        if (QDialog::Accepted == dlg.exec())
        {
            bnb::string_type password = From_QString(dlg.GetPassword());
            if (password.empty())
            {
                HintDialog(hint_type::ht_error, "Please input a password !", this).exec();
                return;
            }

            if (!bnb::Credential::Decoding(_xml, (const unsigned char*)password.c_str(), password.size() * sizeof(bnb::char_type)))
            {
                HintDialog(hint_type::ht_error, "You input password error !", this).exec();
                return;
            }

            g_Credential().SetWord(password);

            if (!g_Credential().FromXml(_xml))
            {
                HintDialog(hint_type::ht_error, "Anaylze file failed !", this).exec();
                return;
            }

            m_strFile = strFile;

            ClearCredential();
            AddCredential();
        }
    }
}

void MainView::OnClickedAbout()
{
    AboutDialog().exec();
}

bool MainView::SwitchNode(unsigned int eType, unsigned int id)
{
    return (_ui.m_viewStack->SwitchToView(static_cast<bnb::credential_enum>(eType), id));
}

bool MainView::OnAddPlatform(unsigned int id1)
{
    if (id1 == g_Credential().GetID())
    {
        EditPlatformDialog dlg(g_Credential(), nullptr, this);
        if (QDialog::Accepted == dlg.exec())
        {
            SaveCredential();

            _ui.m_treeView->AddPlatform(*dlg.GetPlatform());
            _ui.m_viewStack->AddPlatform(*dlg.GetPlatform());
        }

        return true;
    }

    return false;
}

bool MainView::OnAddAccount(unsigned int id1, unsigned int id2)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().FindByID(id2))
        {
            EditAccountDialog dlg(*ptr_platform, nullptr, this);
            if (QDialog::Accepted == dlg.exec())
            {
                SaveCredential();

                _ui.m_treeView->AddAccount(*dlg.GetAccount());
                _ui.m_viewStack->AddAccount(*dlg.GetAccount());
            }

            return true;
        }
    }

    return false;
}

bool MainView::OnAddProperty(unsigned int id1, unsigned int id2, unsigned int id3)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().FindByID(id2, id3))
        {
            EditPropertyDialog dlg(*ptr_account, nullptr, this);
            if (QDialog::Accepted == dlg.exec())
            {
                SaveCredential();

                _ui.m_treeView->AddProperty(*dlg.GetProperty());
                _ui.m_viewStack->AddProperty(*dlg.GetProperty());
            }

            return true;
        }
    }
    
    return false;
}

bool MainView::OnUpdatePassword(unsigned int id1)
{
    if (id1 == g_Credential().GetID())
    {
        EditPasswordDialog dlg(g_Credential(), this);
        if (QDialog::Accepted == dlg.exec())
        {
            SaveCredential();

            _ui.m_viewStack->UpdateCredential(id1);
        }

        return true;
    }

    return false;
}

bool MainView::OnUpdateCredential(unsigned int id1)
{
    if (id1 == g_Credential().GetID())
    {
        EditCredentialDialog dlg(g_Credential(), this);
        if (QDialog::Accepted == dlg.exec())
        {
            SaveCredential();

            _ui.m_treeView->UpdateCredential(id1, g_Credential());
            _ui.m_viewStack->UpdateCredential(id1);
        }

        return true;
    }

    return false;
}

bool MainView::OnUpdatePlatform(unsigned int id1, unsigned int id2)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().FindByID(id2))
        {
            EditPlatformDialog dlg(g_Credential(), ptr_platform, this);
            if (QDialog::Accepted == dlg.exec())
            {
                SaveCredential();

                _ui.m_treeView->UpdatePlatform(*dlg.GetPlatform());
                _ui.m_viewStack->UpdatePlatform(id1, id2);
            }

            return true;
        }
    }

    return false;
}

bool MainView::OnUpdateAccount(unsigned int id1, unsigned int id2, unsigned int id3)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().FindByID(id2))
        {
            if (auto ptr_account = ptr_platform->FindByID(id3))
            {
                EditAccountDialog dlg(*ptr_platform, ptr_account, this);
                if (QDialog::Accepted == dlg.exec())
                {
                    SaveCredential();

                    _ui.m_treeView->UpdateAccount(*dlg.GetAccount());
                    _ui.m_viewStack->UpdateAccount(id1, id2, id3);
                }
                return true;
            }
        }
    }

    return false;
}

bool MainView::OnUpdateProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().FindByID(id2, id3))
        {
            if (auto ptr_property = ptr_account->FindByID(id4))
            {
                EditPropertyDialog dlg(*ptr_account, ptr_property, this);
                if (QDialog::Accepted == dlg.exec())
                {
                    SaveCredential();

                    _ui.m_treeView->UpdateProperty(*dlg.GetProperty());
                    _ui.m_viewStack->UpdateProperty(id1, id3, id4);
                }

                return true;
            }
        }
    }

    return false;
}

bool MainView::OnRemovePlatform(unsigned int id1, unsigned int id2)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().FindByID(id2))
        {
            std::vector<unsigned int> vtrIds{ ptr_platform->GetID() };
            ptr_platform->PreorderTraversal([&vtrIds](const bnb::account_node& account) mutable {
                vtrIds.push_back(account.GetID());
            });

            if (g_Credential().Remove(id2))
            {
                SaveCredential();

                _ui.m_treeView->RemovePlatform(id1, id2);
                _ui.m_viewStack->RemovePlatform(id1, id2, vtrIds);

                return true;
            }
        }
    }

    return false;
}

bool MainView::OnRemoveAccount(unsigned int id1, unsigned int id2, unsigned int id3)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_platform = g_Credential().FindByID(id2))
        {
            if (auto ptr_account = ptr_platform->FindByID(id3))
            {
                std::vector<unsigned int> vtrIds{ ptr_account->GetID() };
                ptr_account->PreorderTraversal([&vtrIds](const bnb::property_node& property) mutable {
                    vtrIds.push_back(property.GetID());
                });

                if (ptr_platform->Remove(id3))
                {
                    SaveCredential();

                    _ui.m_treeView->RemoveAccount(id1, id2, id3);
                    _ui.m_viewStack->RemoveAccount(id1, id2, id3, vtrIds);

                    return true;
                }
            }
        }
    }

    return false;
}

bool MainView::OnRemoveProperty(unsigned int id1, unsigned int id2, unsigned int id3, unsigned int id4)
{
    if (id1 == g_Credential().GetID())
    {
        if (auto ptr_account = g_Credential().FindByID(id2, id3))
        {
            if (ptr_account->Remove(id4))
            {
                SaveCredential();

                _ui.m_treeView->RemoveProperty(id1, id2, id3, id4);
                _ui.m_viewStack->RemoveProperty(id1, id3, id4);

                return true;
            }
        }
    }

    return false;
}

void MainView::ui_type::SetupUI(MainView* pView)
{
    pView->setObjectName("MainView");
    pView->setWindowTitle("Credential Manager");
    ui_utils::SetBackgroundColor(pView, ui_utils::g_clrManView);

    QSplitter* phSplitter = new QSplitter(Qt::Horizontal, pView);
    phSplitter->setObjectName("MainSplitter");
    phSplitter->setHandleWidth(1);
    phSplitter->setOpaqueResize(true);
    phSplitter->setChildrenCollapsible(false);

    m_treeView = new TreeView(pView, phSplitter);

    m_viewStack = new StackView(phSplitter);

    m_viewToolBar = new ToolBar(pView, pView);

    QVBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(1);
    pMainLayout->addWidget(m_viewToolBar);
    pMainLayout->addWidget(phSplitter, 1);

    pView->setLayout(pMainLayout);

    RetranslateUI(pView);
}

void MainView::ui_type::RetranslateUI(MainView * pView)
{

}

void Init()
{
    MainView* _viewMain = new MainView;
    _viewMain->show();
}

QT_END_NAMESPACE

bnb::string_type From_QString(const QT_PREPEND_NAMESPACE(QString)& str)
{
    return str.toStdWString();
}

QT_PREPEND_NAMESPACE(QString) To_QString(const bnb::string_type& str)
{
    return QT_PREPEND_NAMESPACE(QString)::fromStdWString(str);
}
