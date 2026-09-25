/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "../PolicyWatcher.hh"

using namespace Napi;

PolicyWatcher::PolicyWatcher(std::string productName, const Function &okCallback)
    : AsyncProgressQueueWorker(okCallback),
      productName(productName)
{
}

PolicyWatcher::~PolicyWatcher()
{
}

void PolicyWatcher::AddStringPolicy(const std::string name) {}
void PolicyWatcher::AddNumberPolicy(const std::string name) {}
void PolicyWatcher::AddBooleanPolicy(const std::string name) {}
void PolicyWatcher::AddUnionPolicy(const std::string name, const std::vector<std::string> &types) {}
void PolicyWatcher::OnExecute(Napi::Env env)
{
  AsyncProgressQueueWorker::OnExecute(env);
}

void PolicyWatcher::Execute(const ExecutionProgress &progress)
{
  // Send a single-item update (count=1) so AsyncProgressQueueWorker guarantees
  // OnProgress is invoked. The pointer value is never dereferenced because
  // OnProgress ignores the policies array entirely.
  const Policy *dummy = nullptr;
  progress.Send(&dummy, 1);
}

void PolicyWatcher::OnProgress(const Policy *const *policies, size_t count)
{
  // Fire the JS callback with an empty object so the caller's Promise resolves.
  HandleScope scope(Env());
  auto result = Object::New(Env());
  Callback().Call(Receiver().Value(), {result});
}

void PolicyWatcher::OnOK() {}
void PolicyWatcher::Dispose() {}
