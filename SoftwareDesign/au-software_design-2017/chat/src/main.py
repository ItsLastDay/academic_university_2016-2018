from concurrent import futures
import time
import curses
import sys
from asyncio import Future

import grpc

import chat_pb2
import chat_pb2_grpc


SERVER_PORT = '49235'


class ChatReader:
    
    def __init__(self, name):
        print('Creating reader {}'.format(name))
        self.name = name

    def __next__(self):
        s = input()
        msg = '{}:{}: {}'.format(time.time(), self.name, s)

        return chat_pb2.ChatMsg(msg=msg)


class ChatServicer(chat_pb2_grpc.ChatServicer):

    def __init__(self, chat_reader):
        self.reader = chat_reader

    def ChatSession(self, request, context):
        for msg in request:
            print('recv', msg)
            yield next(self.reader)


def main(argv):
    mode = argv[1]
    nick = argv[2]

    chat_reader = ChatReader(nick)

    if mode == 'SERVER':
        server = grpc.server(futures.ThreadPoolExecutor(max_workers=3))
        chat_pb2_grpc.add_ChatServicer_to_server(ChatServicer(chat_reader), server)
        server.add_insecure_port('[::]:{}'.format(SERVER_PORT))
        print('Starting server\n')
        server.start()
        try:
            while True:
                pass
        except KeyboardInterrupt:
            server.stop(0)
    elif mode == 'CLIENT':
        print('Starting client\n')
        channel = grpc.insecure_channel('localhost:{}'.format(SERVER_PORT))
        stub = chat_pb2_grpc.ChatStub(channel)
        for x in stub.ChatSession(chat_reader):
            print('Received msg: {}'.format(x))
    else:
        print('Wrong arguments')
        return 1


    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
