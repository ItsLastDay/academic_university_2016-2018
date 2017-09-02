"""
Module that acts as a repository for all game events.
"""
import logging

class EventHistory:
    """Holder for all events which we'll show to user.
    """

    events = []

    @staticmethod
    def add_event(event_description):
        EventHistory.events.append(event_description)
        logging.info(event_description)

    @staticmethod
    def get_last_n_events(n):
        return EventHistory.events[-n:]
