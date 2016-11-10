#include <string>

#include "credential_qt_manager.h"
#include "credential_view_manager.h"
#include "credential_model_manager.h"

void CredentialManagerQT::Initialize()
{
	Model().Initialize();
	View().Initialize();
}

CredentialViewManager & CredentialManagerQT::View()
{
    return CredentialViewManager::Instance();
}

CredentialModelManager & CredentialManagerQT::Model()
{
    return CredentialModelManager::Instance();
}