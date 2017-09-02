"""A module with Parser responsibility.

Parsing is one of the later steps in
program compilation or intepreting.

It ensures that the stream of lexemes
form a valid program. The result
is a tree that represents a program.

In our case, the result will be
:class:`commands.RunnableCommand`.
"""
from cli.commands import CommandChainPipe, CommandAssignment
from cli.single_command import SingleCommandFactory
from cli.exceptions import ParseException
from cli.lexer import LexemType


class Parser:
    """A static class for parsing a list of lexemes.

    Parser ensures that a stream of lexemes
    match a syntactic structure of a valid command.
    It also builds a representation of this
    command alongway.
    """

    @staticmethod
    def build_command(lexemes):
        """Build :class:`commands.RunnableCommand` out of list of lexemes.

        Our grammar is as following::

            <start> ::= <command> (PIPE <command>)*
            <command> ::= <assignment> | <single_command>
            <assignment> ::= ASSIGNMENT
            <single_command> ::= STRING (STRING | QUOTED_STRING | ASSIGNMENT)*

        where ASSIGNMENT, QUOTED_STRING, STRING and PIPE are lexemes.

        Every rule is implemented as a static method with name _parse_`smth`.
        It returns a pair:

            - a resulting :class:`commands.RunnableCommand`
            - a list of unparsed lexemes
        """
        runnable, unparsed_lexemes = Parser._parse_start(lexemes)

        if unparsed_lexemes:
            raise ParseException('Not all lexemes were parsed. The first starts '\
                                 'at {}'.format(unparsed_lexemes[0].get_position()))

        return runnable

    @staticmethod
    def first_lex_matches_type(lexemes, tp):
        return lexemes and lexemes[0].get_type() == tp
            
    @staticmethod
    def _consume_one_lexem(lexem_list, desired_lexem_type):
        """Consume a lexem of the desired type. Return list of lexemes without consumed one.

        Raises:
            ParseException, if the list is empty or the first lexem is not
                of a type `desired_lexem_type`.
        """
        if not lexem_list:
            raise ParseException('Expected lexem of type {}, found ' \
                                 'none.'.format(desired_lexem_type.name))

        if lexem_list[0].get_type() != desired_lexem_type:
            raise ParseException('Expected lexem of type {}, found '\
                                 'lexem of type {}.'.format(desired_lexem_type.name,
                                                            lexem_list[0].get_type().name))

        return lexem_list[1:]

    @staticmethod
    def _parse_start(lexemes):
        first_command, unparsed_lexemes = Parser._parse_command(lexemes)

        result_command = first_command
        while unparsed_lexemes:
            unparsed_lexemes = Parser._consume_one_lexem(unparsed_lexemes, LexemType.PIPE)
            current_command, unparsed_lexemes = Parser._parse_command(unparsed_lexemes)

            result_command = CommandChainPipe(result_command, current_command)

        return result_command, unparsed_lexemes

    @staticmethod
    def _parse_command(lexemes):
        if Parser.first_lex_matches_type(lexemes, LexemType.ASSIGNMENT):
            return Parser._parse_assignment(lexemes)

        return Parser._parse_single_command(lexemes)


    @staticmethod
    def _parse_assignment(lexemes):
        unprocessed_lexemes = Parser._consume_one_lexem(lexemes, LexemType.ASSIGNMENT)
        command = CommandAssignment([lexemes[0].get_value()])
        return command, unprocessed_lexemes

    @staticmethod
    def _parse_single_command(lexemes):
        rest_lexemes = Parser._consume_one_lexem(lexemes, LexemType.STRING)

        cmd_name_and_args = [lexemes[0]]
        while rest_lexemes:
            first_lex_type = rest_lexemes[0].get_type()

            if first_lex_type in (LexemType.QUOTED_STRING, LexemType.STRING,
                                  LexemType.ASSIGNMENT):
                cmd_name_and_args.append(rest_lexemes[0])
                rest_lexemes = Parser._consume_one_lexem(rest_lexemes, first_lex_type)
            else:
                break

        command = SingleCommandFactory.build_command(cmd_name_and_args)
        return command, rest_lexemes
