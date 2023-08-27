# this program will return specific information on one equity symbol

# imports
import streamlit as st
import requests
from accountInfo import key

# introduction text
st.title("Stock Terminal App")
st.write("This app will display fundamental and technical data of the provided stock symbol.")

# data values
dataValues = {'Last price': 'lastPrice', 'Percent change': 'netPercentChangeInDouble',
              'Float (in millions)': 'marketCapFloat', 'Short interest ratio': 'shortIntToFloat', 'PE Ratio': 'peRatio',
              'Book value per share': 'bookValuePerShare', 'Description': 'description'}

# urls
quotesURL = 'https://api.tdameritrade.com/v1/marketdata/quotes'
searchInstrumentsURL = 'https://api.tdameritrade.com/v1/instruments'

# ask for user input
symbol = st.text_input("Enter a stock symbol: ")

# uppercase user input
symbol = symbol.upper()

# define endpoint parameters
payloadQuotes = {'apikey': key,
           'symbol': symbol}

payloadSearchInstruments = {'apikey': key,
           'symbol': symbol,
           'projection':'fundamental'}

# request data
searchInstruments = requests.get(searchInstrumentsURL,params = payloadSearchInstruments)
quotes = requests.get(quotesURL,params = payloadQuotes)

# convert to dictionary
quotes = quotes.json()
searchInstruments = searchInstruments.json()

# validate symbol and do not allow multiple symbols
while (bool(quotes) != 1 and bool(searchInstruments != 1)) or (',' in symbol) or ('error' in quotes and 'error' in searchInstruments):
    symbol = input('\nInvalid equity symbol, please try again: ')
    symbol = symbol.upper()

    # define endpoint parameters
    payloadQuotes = {'apikey': key,
                     'symbol': symbol}

    payloadSearchInstruments = {'apikey': key,
                                'symbol': symbol,
                                'projection': 'fundamental'}

    # request data
    searchInstruments = requests.get(searchInstrumentsURL, params=payloadSearchInstruments)
    quotes = requests.get(quotesURL, params=payloadQuotes)

    # convert to dictionary
    quotes = quotes.json()
    searchInstruments = searchInstruments.json()

# unpack and combine data
# quotes dictionary has 1 nested dictionary
# searchInstruments has 2 nested dictionaries
searchInstruments = searchInstruments[symbol]
symbolData = {**quotes[symbol], **searchInstruments['fundamental']}

# display data with button
if st.button("Display data"):
    for label, dataCode in dataValues.items():
        st.write(label + ': ' + str(symbolData[dataCode]))
