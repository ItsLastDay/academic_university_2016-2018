from collections import deque

from fa import FiniteAutomata
from dfa import DFA


class NFA(FiniteAutomata):
    """A non-deterministic finite automata with eps-jumps.
    """

    EPS = None

    def check_self(self):
        for state in self.states:
            for ch in self.alphabet:
                jmp_key = (state, ch)
                
                jmp_to = self.transitions.get(jmp_key, [])
                assert len(set(jmp_to)) == len(jmp_to)

        for key in self.transitions:
            fr_state, letter = key
            assert letter is NFA.EPS or letter in self.alphabet

    def _get_epsilon_closure(self, state):
        closure_states = {state}
        bfs = deque([state])

        while bfs:
            cur_state = bfs.popleft()

            for next_state in self.transitions.get((cur_state, NFA.EPS), []):
                if next_state in closure_states:
                    continue
                closure_states.add(next_state)
                bfs.append(next_state)

        return closure_states

    def precalc(self):
        self.state_to_eps_closure = dict()
        for state in self.states:
            self.state_to_eps_closure[state] = self._get_epsilon_closure(state)

        self.transitions_closured = dict()
        for state in self.states:
            for ch in self.alphabet:
                next_states = self.transitions.get((state, ch), [])

                closured_states = set()
                for next_state in next_states:
                    closured_states.update(self.state_to_eps_closure[next_state])

                self.transitions_closured[(state, ch)] = list(closured_states)

    def accept_word(self, word):
        cur_states = self.state_to_eps_closure[self.initial_state]

        for ch in word:
            if ch not in self.alphabet:
                return False

            next_states = set()
            for cur_state in cur_states:
                next_states.update(set(self.transitions_closured[(cur_state, ch)]))
            cur_states = next_states

        return len(cur_states.intersection(self.terminal_states)) != 0

    def to_nfa_without_eps_jumps(self):
        old_to_new_state = dict()
        old_to_new_state_set = dict()
        new_states = set()
        new_terminal = set()

        for state in self.states:
            new_state = self.state_to_eps_closure[state]
            new_state_name = self.combine_names(new_state)
            new_states.add(new_state_name)
            old_to_new_state[state] = new_state_name
            old_to_new_state_set[state] = new_state

            if new_state.intersection(self.terminal_states):
                new_terminal.add(new_state_name)

        new_transitions = dict()
        for state in self.states: 
            for ch in self.alphabet:
                new_to_states = []

                for cur_state in old_to_new_state_set[state]:
                    for next_state_plain in self.transitions.get((cur_state, ch), []):
                        next_state = old_to_new_state[next_state_plain]
                        new_to_states.append(next_state)

                new_transitions[(old_to_new_state[state], ch)] = list(set(new_to_states))

        return NFA(new_states,
                old_to_new_state[self.initial_state],
                new_terminal,
                new_transitions,
                self.alphabet)

    def to_dfa(self):
        # Easier to think when no eps-jumps are present.
        self = self.to_nfa_without_eps_jumps()

        initial_set = frozenset([self.initial_state])
        new_states = {self.initial_state} 
        new_states_sets = {initial_set}
        new_terminal = set()
        new_transitions = dict()

        bfs = deque([initial_set])

        while bfs:
            cur_state_set = bfs.popleft()

            # Build transitions from the current state.
            for ch in self.alphabet:
                to_states_set = set()

                for cur_state in cur_state_set:
                    to_states_set.update(set(self.transitions.get((cur_state, ch), [])))

                if not to_states_set:
                    continue

                to_states_set = frozenset(to_states_set)
                cur_state = self.combine_names(cur_state_set)
                to_states_name = self.combine_names(to_states_set)

                new_transitions[(cur_state, ch)] = [to_states_name]

                if to_states_set not in new_states_sets:
                    new_states_sets.add(to_states_set)
                    bfs.append(to_states_set)
                    new_states.add(to_states_name)

                    if to_states_set.intersection(self.terminal_states):
                        new_terminal.add(to_states_name)

        return DFA(new_states,
                self.initial_state,
                new_terminal,
                new_transitions,
                self.alphabet)
