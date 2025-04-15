// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/fml/sub_queue_task_runner.h"
#include "flutter/fml/memory/task_runner_checker.h"
#include "flutter/fml/message_loop_impl.h"

namespace fml {

SubQueueTaskRunner::SubQueueTaskRunner()
    : fml::TaskRunner(nullptr),
      task_queue_(MessageLoopTaskQueues::GetInstance()),
      queue_id_(task_queue_->CreateTaskQueue()) {}

SubQueueTaskRunner::~SubQueueTaskRunner() = default;

fml::RefPtr<fml::TaskRunner> SubQueueTaskRunner::Create(
    fml::RefPtr<fml::TaskRunner> parent) {
  auto task_runner = fml::MakeRefCounted<SubQueueTaskRunner>();
  task_runner->task_queue_->Merge(parent->GetTaskQueueId(),
                                  task_runner->queue_id_);
  return task_runner;
}

void SubQueueTaskRunner::PostTask(const fml::closure& task) {
  task_queue_->RegisterTask(queue_id_, task, fml::TimePoint::Now());
}

void SubQueueTaskRunner::PostTaskForTime(const fml::closure& task,
                                         fml::TimePoint target_time) {
  task_queue_->RegisterTask(queue_id_, task, target_time);
}

void SubQueueTaskRunner::PostDelayedTask(const fml::closure& task,
                                         fml::TimeDelta delay) {
  task_queue_->RegisterTask(queue_id_, task, fml::TimePoint::Now() + delay);
}

bool SubQueueTaskRunner::RunsTasksOnCurrentThread() {
  if (!fml::MessageLoop::IsInitializedForCurrentThread()) {
    return false;
  }
  return TaskRunnerChecker::RunsOnTheSameThread(
      MessageLoop::GetCurrentTaskQueueId(), queue_id_);
}

fml::TaskQueueId SubQueueTaskRunner::GetTaskQueueId() {
  return queue_id_;
}

}  // namespace fml
