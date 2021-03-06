//
// MidiSequenceTest.c - MrsWatson
// Copyright (c) 2016 Teragon Audio. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include "midi/MidiSequence.h"

#include "unit/TestRunner.h"

static int _testNewMidiSequence(void) {
  MidiSequence m = newMidiSequence();
  assertNotNull(m);
  assertIntEquals(0, linkedListLength(m->midiEvents));
  freeMidiSequence(m);
  return 0;
}

static int _testAppendMidiEventToSequence(void) {
  MidiSequence m = newMidiSequence();
  MidiEvent e = newMidiEvent();
  appendMidiEventToSequence(m, e);
  assertIntEquals(1, linkedListLength(m->midiEvents));
  freeMidiSequence(m);
  return 0;
}

static int _testAppendNullMidiEventToSequence(void) {
  MidiSequence m = newMidiSequence();
  appendMidiEventToSequence(m, NULL);
  assertIntEquals(0, linkedListLength(m->midiEvents));
  freeMidiSequence(m);
  return 0;
}

static int _testAppendEventToNullSequence(void) {
  // Test is not crashing
  MidiEvent e = newMidiEvent();
  appendMidiEventToSequence(NULL, e);
  freeMidiEvent(e);
  return 0;
}

static int _testFillMidiEventsFromRangeStart(void) {
  MidiSequence m = newMidiSequence();
  MidiEvent e = newMidiEvent();
  LinkedList l = newLinkedList();

  e->status = 0xf7;
  e->timestamp = 100;
  appendMidiEventToSequence(m, e);
  assertFalse(fillMidiEventsFromRange(m, 0, 256, l));
  assertIntEquals(1, linkedListLength(l));
  assertIntEquals(0xf7, ((MidiEvent)l->item)->status);

  freeMidiSequence(m);
  freeLinkedList(l);
  return 0;
}

static int _testFillEventsFromEmptyRange(void) {
  MidiSequence m = newMidiSequence();
  MidiEvent e = newMidiEvent();
  LinkedList l = newLinkedList();

  e->status = 0xf7;
  e->timestamp = 100;
  appendMidiEventToSequence(m, e);
  assert(fillMidiEventsFromRange(m, 0, 0, l));
  assertIntEquals(0, linkedListLength(l));

  freeMidiSequence(m);
  freeLinkedList(l);
  return 0;
}

static int _testFillEventsSequentially(void) {
  MidiSequence m = newMidiSequence();
  MidiEvent e = newMidiEvent();
  MidiEvent e2 = newMidiEvent();
  LinkedList l = newLinkedList();

  e->status = 0xf7;
  e->timestamp = 100;
  e2->status = 0xf7;
  e2->timestamp = 300;
  appendMidiEventToSequence(m, e);
  appendMidiEventToSequence(m, e2);
  assert(fillMidiEventsFromRange(m, 0, 256, l));
  assertIntEquals(1, linkedListLength(l));
  freeLinkedList(l);
  l = newLinkedList();
  assertFalse(fillMidiEventsFromRange(m, 256, 256, l));
  assertIntEquals(1, linkedListLength(l));

  freeMidiSequence(m);
  freeLinkedList(l);
  return 0;
}

static int _testFillEventsFromRangePastSequence(void) {
  MidiSequence m = newMidiSequence();
  MidiEvent e = newMidiEvent();
  LinkedList l = newLinkedList();

  e->status = 0xf7;
  e->timestamp = 100;
  appendMidiEventToSequence(m, e);
  // Should return false since this is the last event in the sequence
  assertFalse(fillMidiEventsFromRange(m, 0, 200, l));
  assertIntEquals(1, linkedListLength(l));
  freeLinkedList(l);
  l = newLinkedList();
  assertFalse(fillMidiEventsFromRange(m, 200, 256, l));
  assertIntEquals(0, linkedListLength(l));

  freeMidiSequence(m);
  freeLinkedList(l);
  return 0;
}

TestSuite addMidiSequenceTests(void);
TestSuite addMidiSequenceTests(void) {
  TestSuite testSuite = newTestSuite("MidiSequence", NULL, NULL);

  addTest(testSuite, "Initialization", _testNewMidiSequence);
  addTest(testSuite, "AppendEvent", _testAppendMidiEventToSequence);
  addTest(testSuite, "AppendNullEvent", _testAppendNullMidiEventToSequence);
  addTest(testSuite, "AppendEventToNullSequence",
          _testAppendEventToNullSequence);
  addTest(testSuite, "FillEventsFromRangeStart",
          _testFillMidiEventsFromRangeStart);
  addTest(testSuite, "FillEventsFromEmptyRange", _testFillEventsFromEmptyRange);
  addTest(testSuite, "FillEventsSequentially", _testFillEventsSequentially);
  addTest(testSuite, "FillEventsFromRangePastSequenceEnd",
          _testFillEventsFromRangePastSequence);

  return testSuite;
}
