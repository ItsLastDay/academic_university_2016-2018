from collections import deque, defaultdict
import copy

from fa import FiniteAutomata


class DFA(FiniteAutomata):
    """Deterministic finite automaton.

    Supports minimization and completion automaton.
    """

    def accept_word(self, word):
        cur_state = self.initial_state
        
        for ch in word:
            if ch not in self.alphabet:
                return False

            transition_key = (cur_state, ch)

            if transition_key not in self.transitions:
                return False

            jump_to = self.transitions[transition_key]

            cur_state = jump_to[0]

        return cur_state in self.terminal_states

    def _get_reachable_states(self, state):
        reachable = {state}
        bfs = deque([state])

        while bfs:
            cur_state = bfs.popleft()

            for ch in self.alphabet:
                transition_key = (cur_state, ch)
                for to_state in self.transitions.get(transition_key, []):
                    if to_state in reachable:
                        continue
                    reachable.add(to_state)
                    bfs.append(to_state)

        return reachable

    def _get_reachable_from_start(self):
        return self._get_reachable_states(self.initial_state)

    def _filter_unreachable_end(self, states):
        end_reachable_from_them = set()

        for state in states:
            reachable_from_here = self._get_reachable_states(state)

            if reachable_from_here.intersection(self.terminal_states):
                end_reachable_from_them.add(state)

        return end_reachable_from_them

    def to_complete_dfa(self):
        need_sink = False
        sink_name = self.gen_random_name('sink_')
        for state in self.states:
            for ch in self.alphabet:
                transition_key = (state, ch)
                
                if transition_key not in self.transitions:
                    need_sink = True

        if need_sink:
            new_transitions = copy.copy(self.transitions)
            new_states = copy.copy(self.states)
            new_states.add(sink_name)

            for state in self.states:
                for ch in self.alphabet:
                    transition_key = (state, ch)
                    
                    if transition_key not in self.transitions:
                        new_transitions[transition_key] = [sink_name]

            for ch in self.alphabet:
                new_transitions[(sink_name, ch)] = [sink_name]
        else:
            return self

        return DFA(new_states,
                   self.initial_state,
                   self.terminal_states,
                   new_transitions,
                   self.alphabet)

    def to_dfa_without_unreachable(self):
        """Erase states that are unreachable from start
        or those, from which terminal state is not reachable.
        """
        reachable_from_start = self._get_reachable_from_start()
        end_reachable_from_them = self._filter_unreachable_end(reachable_from_start)

        if self.initial_state not in end_reachable_from_them:
            return DFA(set('sink'),
                       'sink',
                       set(),
                       dict(),
                       self.alphabet)

        new_transitions = dict()
        new_terminal = end_reachable_from_them.intersection(self.terminal_states)
        for new_state in end_reachable_from_them:
            for ch in self.alphabet:
                transition_key = (new_state, ch)
                
                if transition_key not in self.transitions:
                    continue

                jump_to = self.transitions[transition_key][0]
                if jump_to not in end_reachable_from_them:
                    continue

                new_transitions[(new_state, ch)] = [jump_to]

        return DFA(end_reachable_from_them,
                   self.initial_state,
                   new_terminal,
                   new_transitions,
                   self.alphabet)

    def to_minimal_dfa(self):
        self = self.to_complete_dfa()

        inverse_transitions = defaultdict(list)
        for key, to_states in self.transitions.items():
            fr_state, ch = key

            for to_state in to_states:
                inverse_transitions[(to_state, ch)].append(fr_state)

        distinguishable_pairs = {(u, v) for u in self.states
                                 for v in self.states
                                 if (u in self.terminal_states) ^ (v in self.terminal_states)}
        bfs = deque(distinguishable_pairs)
 
        while bfs:
            cur_pair = bfs.popleft()
            first_state, sec_state = cur_pair

            for ch in self.alphabet:
                for first_from in inverse_transitions.get((first_state, ch), []):
                    for sec_from in inverse_transitions.get((sec_state, ch), []):
                        next_pair = (first_from, sec_from)

                        if next_pair in distinguishable_pairs:
                            continue

                        distinguishable_pairs.add(next_pair)
                        bfs.append(next_pair)


        minimized_states = set()
        terminal_minimized_states = set()
        state_to_minimized_state = dict()
        for state in self.states:
            minimized_state = set()
            assert (state, state) not in distinguishable_pairs
            
            for other_state in self.states:
                if (state, other_state) not in distinguishable_pairs:
                    minimized_state.add(other_state)

            state_set = minimized_state
            minimized_state = self.combine_names(minimized_state)
            state_to_minimized_state[state] = minimized_state
            minimized_states.add(minimized_state)

            if state_set.intersection(self.terminal_states):
                terminal_minimized_states.add(minimized_state)

        minimized_transitions = dict()
        for state in self.states:
            fr_min_state = state_to_minimized_state[state]
            for ch in self.alphabet:
                transition_key = (state, ch)
                
                if transition_key not in self.transitions:
                    continue

                jump_to = self.transitions[transition_key]
                
                to_state = jump_to[0]

                if to_state not in state_to_minimized_state:
                    # Transition goes to an invalid state (like unreachable).
                    continue

                to_min_state = state_to_minimized_state[to_state]

                minimized_transitions[(fr_min_state, ch)] = [to_min_state]

        return DFA(minimized_states,
                   state_to_minimized_state[self.initial_state],
                   terminal_minimized_states,
                   minimized_transitions,
                   self.alphabet).to_dfa_without_unreachable()

    def check_self(self):
        for key in self.transitions:
            fr_state, letter = key
            assert letter in self.alphabet

        for state in self.states:
            for ch in self.alphabet:
                transition_key = (state, ch)
                
                if transition_key not in self.transitions:
                    continue

                jump_to = self.transitions[transition_key]
                assert len(jump_to) == 1
