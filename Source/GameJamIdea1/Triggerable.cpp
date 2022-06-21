// Fill out your copyright notice in the Description page of Project Settings.

#include "Triggerable.h"

// Add default functionality here for any ITriggerable functions that are not pure virtual.

bool ITriggerable::Trigger(AActor* Trigger) {
  if (GetUniqueTriggers().Contains(Trigger)) {
    return false;
  }
  GetUniqueTriggers().AddUnique(Trigger);
  MarkDirty();
  return true;
}

bool ITriggerable::UnTrigger(AActor* Trigger) {
  if (!GetUniqueTriggers().Contains(Trigger)) {
    return false;
  }
  GetUniqueTriggers().Remove(Trigger);
  MarkDirty();
  return true;
}
