#!/usr/bin/env python
#
# Copyright (c) 2008, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ---
#
# Create a state machine object based on a definition file.
#

__author__ = 'falmeida@google.com (Filipe Almeida)'

class OrderedDict:
  """Ordered dictionary implementation."""

  # Define the minimum functionality we need for our application.
  # Easiser would be to subclass from UserDict.DictMixin, and only
  # define __getitem__, __setitem__, __delitem__, and keys, but that's
  # not as portable.  We don't need to define much more, so we just do.

  def __init__(self):
    self._dict = {}
    self._keys = []

  def __getitem__(self, key):
    return self._dict[key]

  def __setitem__(self, key, value):
    if key not in self._keys:
      self._keys.append(key)
    self._dict[key] = value

  def __delitem__(self, key):
    self._keys.remove(key)
    del self._dict[key]

  def keys(self):
    return self._keys

  # Below are all we have to define in addition to what DictMixin would need
  def __len__(self):
    return len(self.keys())

  def __contains__(self, key):
    return self.has_key(key)

  def __iter__(self):
    # It's not as portable -- though it would be more space-efficient -- to do
    #   for k in self.keys(): yield k
    return iter(self.keys())

class State(object):
  """Contains information about a specific state."""

  def __init__(self):
    pass

  name = None
  external_name = None
  transitions = []


class Transition(object):
  """Contains information about a specific transition."""

  def __init__(self, condition, source, destination):
    self.condition = condition
    self.source = source
    self.destination = destination


class FSMConfig(object):
  """Container for the statemachine definition."""

  sm = {}  # dictionary that contains the finite state machine definition
           # loaded from a config file.
  transitions = []  # List of transitions.
  conditions = {}   # Mapping between the condition name and the bracket
                    # expression.
  states = OrderedDict()  # Ordered dictionary of states.
  name = None
  comment = None

  def AddState(self, **dic):
    """Called from the definition file with the description of the state.

    Receives a dictionary and populates internal structures based on it. The
    dictionary is in the following format:

    {'name': state_name,
     'external': exposed state name,
     'transitions': [
       [condition, destination_state ],
       [condition, destination_state ]
     ]
    }

    """

    state = State()
    state.name = dic['name']
    state.external_name = dic['external']

    state_transitions = []

    for (condition, destination) in dic['transitions']:
      transition = Transition(condition, state.name, destination)
      state_transitions.append(transition)

    self.transitions.extend(state_transitions)
    state.transitions = state_transitions
    self.states[state.name] = state

  def AddCondition(self, name, expression):
    """Called from the definition file with the definition of a condition.

    Receives the name of the condition and it's expression.
    """
    self.conditions[name] = expression

  def Load(self, filename):
    """Load the state machine definition file.

    In the definition file, which is based on the python syntax, the following
    variables and functions are defined.

    name: Name of the state machine
    comment: Comment line on the generated file.
    condition(): A mapping between condition names and bracket expressions.
    state(): Defines a state and it's transitions. It accepts the following
             attributes:
      name: name of the state
      external: exported name of the state. The exported name can be used
                multiple times in order to create a super state.
      transitions: List of pairs containing the condition for the transition
                   and the destination state. Transitions are ordered so if
                   a default rule is used, it must be the last one in the list.

    Example:

    name = 'c comment parser'

    condition('/', '/')
    condition('*', '*')
    condition('linefeed', '\\n')
    condition('default', '[:default:]')

    state(name = 'text',
          external = 'comment',
          transitions = [
            [ '/', 'comment_start' ],
            [ 'default', 'text' ]
          ])

    state(name = 'comment_start',
          external = 'comment',
          transitions = [
            [ '/', 'comment_line' ],
            [ '*', 'comment_multiline' ],
            [ 'default', 'text' ]
          ])

    state(name = 'comment_line',
          external = 'comment',
          transitions = [
            [ 'linefeed', 'text' ],
            [ 'default', 'comment_line' ]
          ])

    state(name = 'comment_multiline',
          external = 'comment',
          transitions = [
            [ '*', 'comment_multiline_close' ],
            [ 'default', 'comment_multiline' ]
          ])

    state(name = 'comment_multiline_close',
          external = 'comment',
          transitions = [
            [ '/', 'text' ],
            [ 'default', 'comment_multiline' ]
          ])

    """

    self.sm['state'] = self.AddState
    self.sm['condition'] = self.AddCondition
    exec(open(filename).read(), self.sm)
    self.name = self.sm['name']
    if not self.name.isalnum():
      raise Exception("State machine name must consist of only alphanumeric"
                      "characters.")
    self.comment = self.sm['comment']

  def __init__(self):
    pass
