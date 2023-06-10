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

def variation_search(barcode, wholeResult, key, secret):
    for variation in wholeResult['variations']:
        url = f"https://goodnaturehf.com/wp-json/wc/v3/products/{wholeResult['id']}/variations/{variation}?consumer_key={key}&consumer_secret={secret}" 
        res = json.loads(requests.get(url).text)
        pprint(res)



def main():
    key, secret = read_creds()
    barcode = "628110035578"

    res = search(barcode, key, secret)[0]
   
    if res['type'] == 'variable':
        variation_search(barcode, res, key, secret)
    else:
        pprint(res)



if __name__ == "__main__":
    main()
