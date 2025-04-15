// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_FML_SUB_QUEUE_TASK_RUNNER_H_
#define FLUTTER_FML_SUB_QUEUE_TASK_RUNNER_H_

#include "flutter/fml/task_runner.h"

namespace fml {

class SubQueueTaskRunner : public fml::TaskRunner {
 public:
  virtual ~SubQueueTaskRunner();

  static fml::RefPtr<fml::TaskRunner> Create(
      fml::RefPtr<fml::TaskRunner> parent);

 private:
  SubQueueTaskRunner();

  // |fml::TaskRunner|
  void PostTask(const fml::closure& task) override;

  // |fml::TaskRunner|
  void PostTaskForTime(const fml::closure& task,
                       fml::TimePoint target_time) override;

  // |fml::TaskRunner|
  void PostDelayedTask(const fml::closure& task, fml::TimeDelta delay) override;

  // |fml::TaskRunner|
  bool RunsTasksOnCurrentThread() override;

  // |fml::TaskRunner|
  fml::TaskQueueId GetTaskQueueId() override;

  fml::MessageLoopTaskQueues* task_queue_;
  fml::TaskQueueId queue_id_;

  FML_FRIEND_MAKE_REF_COUNTED(SubQueueTaskRunner);
  FML_DISALLOW_COPY_AND_ASSIGN(SubQueueTaskRunner);
};

}  // namespace fml

#endif  // FLUTTER_FML_SUB_QUEUE_TASK_RUNNER_H_
