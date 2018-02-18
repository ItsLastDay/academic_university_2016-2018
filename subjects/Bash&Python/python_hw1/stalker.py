#!/usr/bin/env python3

import requests
import pickle
import sys
import argparse
import os.path


class VkAPIError(Exception):
    pass


def vk_api_call(method, **params):
    params['v'] = '5.60'  # Latest version at the time of writing.
    base_url_fmt = 'https://api.vk.com/method/{}'
    response = requests.get(base_url_fmt.format(method), params=params)
    json = response.json()
    if 'error' in json:
        raise VkAPIError(json['error']['error_msg'])
    return json['response']


def format_user_name(dict_user):
    first_name = dict_user['first_name']
    last_name = dict_user['last_name']
    nickname = dict_user.get('nickname', '')
    nickname = '"{}" '.format(nickname) if nickname else ''
    return '{} {}{}'.format(first_name, nickname, last_name)


def get_filename_by_id(user_id):
    return 'stalker_{}_saved.p'.format(user_id)


def get_friends_vkapi(user_id):
    friends = vk_api_call('friends.get', user_id=user_id, fields='nickname')
    friends_set = set(tuple(sorted(x.items())) for x in friends['items'])
    return friends_set


def get_friends_pickled(user_id):
    friends_filename = get_filename_by_id(user_id)
    if not os.path.isfile(friends_filename):
        raise ValueError("Querying user's friends before saving")
    with open(friends_filename, 'rb') as dump_file:
        return pickle.load(dump_file)


def save(user_id):
    friends = get_friends_vkapi(user_id)
    with open(get_filename_by_id(user_id), 'wb') as dump_file:
        pickle.dump(friends, dump_file)
    print('{} friends successfully saved.'.format(len(friends)))


def print_friends_set(title, friends_set):
    print('\nThere are {} {} friends:'.format(len(friends_set), title))
    for i, friend in enumerate(friends_set, start=1):
        print('{}. {}'.format(i, format_user_name(dict(friend))))
    print('-' * 80)


def diff(user_id):
    friends_before = get_friends_pickled(user_id)
    friends_now = get_friends_vkapi(user_id)

    new_friends = friends_now.difference(friends_before)
    gone_friends = friends_before.difference(friends_now)

    print('User currently has {} friends.'.format(len(friends_now)))

    print_friends_set('Gone', gone_friends)
    print_friends_set('New', new_friends)


def get_user_id(provided_id):
    # If screen name is provided (e.g. "durov" or "id1"), resolve it to id.
    # Otherwise, use it as is.
    try:
        user_id = vk_api_call('utils.resolveScreenName',
                              screen_name=provided_id)
        # If `provided_id` is not a screen name, an empty list will be
        # returned.
        user_id = user_id['object_id']
    except (VkAPIError, TypeError):
        user_id = provided_id

    return user_id


def print_user_info(user_id):
    user_info = vk_api_call('users.get', user_ids=user_id,
                            fields='nickname')[0]
    print('We are watching user {}.\n'.format(format_user_name(user_info)))


def main():
    parser = argparse.ArgumentParser(description="Watch user's friends on VK")
    diff_or_save = parser.add_mutually_exclusive_group(required=True)
    diff_or_save.add_argument('-s', '--save', action='store_true',
                              help="save user's friends")
    diff_or_save.add_argument('-d', '--diff', action='store_true',
                              help='print difference between stored friends and\
                                      current friends')
    parser.add_argument('id', help="watched user's id or screen name")
    args = parser.parse_args()

    user_id = get_user_id(args.id)

    try:
        print_user_info(user_id)

        if args.save:
            save(user_id)
        else:
            diff(user_id)
    except VkAPIError as e:
        print('VK API failed with the following error: {}'.format(e))
        return 1
    except Exception as e:
        print('An exception occured: {}'.format(e))
        return 1

    return 0

if __name__ == '__main__':
    # Example of banned person: 8254567
    # Example of person with nickname: 768557
    sys.exit(main())
