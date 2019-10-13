import requests
import sys
import base64

url = 'http://virustotal.ap-northeast-2.elasticbeanstalk.com/'
submit_path = '/virus/submit'

if len(sys.argv) == 2:
    upload_file = sys.argv[1]
else:
    upload_file = 'solve'

data = base64.b64encode(open(upload_file, 'rb').read()).decode()

print('sending request...')
res = requests.post(f'{url}{submit_path}',
        data={'attach': data}
)
print(res.text)
