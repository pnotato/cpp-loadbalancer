from flask import Flask, request

app = Flask(__name__)

@app.route('/', methods=['GET'])
def output():
    return "Server 3 is running at port 5002!"

if __name__ == '__main__':
    app.run(host='127.0.0.1', port=5002)