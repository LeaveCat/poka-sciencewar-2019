import random
import string
import base64
import subprocess
import os
import stat
import traceback

from flask import Flask
from flask_restful import reqparse, abort, Api, Resource

application = Flask(__name__)
api = Api(application)
PATH = '/tmp/uploads'

def get_random_name():
    return ''.join([random.choice(string.printable[:62]) for _ in range(20)])

class SubmitVirus(Resource):
    def post(self):
        parser = reqparse.RequestParser()
        parser.add_argument('attach')
        args = parser.parse_args()

        virus = args['attach']
        filename = get_random_name()

        target_path = f'{PATH}/{filename}'
        print(target_path)
        with open(target_path, 'wb') as target:
            target.write(base64.b64decode(virus))

        current_stat = os.stat(target_path)
        os.chmod(target_path, current_stat.st_mode | stat.S_IEXEC)
        try:
            output = subprocess.call(f'./runner {target_path}', shell=True)
        except:
            traceback.print_exc()
            data = '500 internal error'
        else:
            data = 'success'

        os.remove(target_path)

        return {'result': data}

api.add_resource(SubmitVirus, '/virus/submit')

if __name__ == '__main__':
    application.run(debug=True)
