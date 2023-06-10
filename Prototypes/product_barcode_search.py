# Search up a product with its barcode.
import requests, json
from pprint import pprint

def read_creds():
    '''
    Read credentials from parent/previous directory
    '''
    with open("../credentials", 'r') as cred:
        cred = cred.read().splitlines()
        return (cred)

def search(barcode, key, secret):
    url = f"https://goodnaturehf.com/wp-json/wc/v3/products?search={barcode}&consumer_key={key}&consumer_secret={secret}"
    return json.loads(requests.get(url).text)


def main():
    key, secret = read_creds()
    
    res = search("032917007384", key, secret)
    pprint(res[0]['price'])



if __name__ == "__main__":
    main()
