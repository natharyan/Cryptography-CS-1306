from collections import Counter
import sys

with open('ref_text.txt','r') as f:
    lines = f.readlines()
    # get a single string
    lines = ''.join(lines)
    # not include any accented characters
    lines = lines.encode('ascii', 'ignore').decode('ascii')
    # convert the list to a string with only non-accented alphabets
    lines = ''.join([i.lower() for i in lines if i.isalpha()])

# create a dictionary with the text characters and their corresponding counts
def getcountdict(l):
    countdict = {}
    # for each character in l
    for i in l:
        # if already exists as a key
        if i in countdict:
            # incremenet its counter
            countdict[i] += 1
        else:
            # else inititialise the key wiht count 1
            countdict[i] = 1
    
    return countdict

# use chi-squared error for better comparison with varying ciphertext length and rare characters than least squares
def chi_squared(observed, expected):
    return sum((observed.get(i, 0) - expected.get(i, 0))**2 / expected.get(i, 1e-10) for i in range(26))

# create dictionary with the character frequencies in the text (or their probability of occurenece based on the input text)
def getfreqfict(count_dict, length):
    freqdict = {}
    for char, count in count_dict.items():
        # map characters a...z as intergers 0...25 for the keys, take its value as the probability of occurrence of the character based on the counter dictionary
        freqdict[ord(char) - ord('a')] = count / length
    return freqdict

# compare the frequencies of the characters in the ciphertext with the reference text, using a shift varying from 0 to 25 for the comparison
def getshifterrors(ref, ciph):
    error_list = []
    for shift in range(26):
        # create a frequency dictionary with shifted keys using (i - shift)%26
        shifted_ciph = {(i-shift)%26: ciph[i] for i in range(26)}
        # calculate the chi-squared error between the reference frequencies and the input shifted frequencies
        error = chi_squared(shifted_ciph, ref)
        # add to the error list for each shift in 0...25
        error_list.append(error)
    return error_list

def getplaintext(ciphertext,key):
    plaintext = ''
    for i in ciphertext:
        # shift back each character of the ciphertext by the key and convert from into character using ascii values
        plaintext += chr((ord(i) - ord('a') - key) % 26 + ord('a'))
    return plaintext

def _getplaintext(ciphtext,ref_freqdict):
    # get the counter dictionary for the ciphertext
    ciph_cntdict = getcountdict(ciphtext)
    # get the frequency dictionary for the ciphertext
    ciph_freqdict = getfreqfict(ciph_cntdict,len(ciphtext))
    # sort the frequency dictionary using the keys
    ciph_freqdict = dict(sorted(ciph_freqdict.items()))
    # for any interger in the range 0...25 not in the frequency dictionary, add it with frequency 0
    for i in range(26):
        if i not in ciph_freqdict:
            ciph_freqdict[i] = 0
    # get the chi-squared errors for each possible key shift
    shifterrors = getshifterrors(ref_freqdict,ciph_freqdict)
    # get the key giving the minimumm error
    key = shifterrors.index(min(shifterrors))
    # return the plaintext decryption and the error with the selected key
    return getplaintext(ciphtext,key),min(shifterrors),key

# calculate the bigram score for the text
def calculate_bigramscore(text):
    # list common English bigrams
    common_bigrams = ['th', 'he', 'in', 'en', 'nt', 're', 'er', 'an', 'ti', 'es', 'on', 'at', 'se', 'nd', 'or', 'ar', 'al', 'te', 'co', 'de', 'to', 'ra', 'et', 'ed', 'it', 'sa', 'em', 'ro']
    
    # get all bigrams in the text
    bigrams = [text[i:i+2] for i in range(len(text)-1)]
    # get the counts of each bigram
    bigram_counts = Counter(bigrams)
    # get the score for each bigram in the common bigrams list
    score = sum(bigram_counts[bigram] for bigram in common_bigrams)
    # return the average score
    return score / len(text)

def calculate_combined_score(plaintext, chi_squared_error):
    bigram_score = calculate_bigramscore(plaintext)
    # normalized chi-squared error (lower is better)
    normalized_chi_squared = 1 / (1 + chi_squared_error)
    # combine scores (higher is better)
    return bigram_score * normalized_chi_squared

def _decrypt(ciphertext):
    best_score = 0
    decrypted_text = ''
    keys = []
    # test for each key length from 1 to 6
    for key_size in range(1, 7):
        keysize_error = 0
        temp_keys = []
        # split the ciphertext into key_size number of subtexts
        ciph_subtexts = ['' for _ in range(key_size)]
        # for each character in the ciphertext, add it to its corresponding subtext based on index % key_size
        for i, char in enumerate(ciphertext):
            ciph_subtexts[i % key_size] += char

        # get the plaintext for each subtext using frequency analysis
        plaintext_subtexts = []
        for sub_ciphtext in ciph_subtexts:
            temp_subtext, subtext_error,sub_key = _getplaintext(sub_ciphtext, ref_freqdict)
            temp_keys.append(sub_key)
            plaintext_subtexts.append(temp_subtext)
            keysize_error += subtext_error

        plaintext = ''
        for i in range(len(ciphertext)):
            plaintext += plaintext_subtexts[i % key_size][i // key_size]

        combined_score = calculate_combined_score(plaintext, keysize_error)
        if combined_score > best_score:
            best_score = combined_score
            decrypted_text = plaintext
            best_key_size = key_size
            keys = list(temp_keys)
    return decrypted_text, best_key_size,keys

if __name__ == "__main__":
    # construct reference frequency table
    # get the character counts
    ref_cntdict = getcountdict(lines)
    # get the frequency table
    ref_freqdict = getfreqfict(ref_cntdict,len(lines))
    # sort it using the keys
    ref_freqdict = dict(sorted(ref_freqdict.items()))

    # ciphertext1 = "qivjukosqegnyiytxypshzewjsnsdpeybsuiranshzewjsnsdvusdvozqhasghexhvtdrynjyirlrrnfpekjbsuhucnjyirlrrnfveylrsdgbinjyirlrrnfwilqbsuqlisfqhhzuxytxaewhroxwvasjirxwsltyiytxontzxhjuyljvenivsdtlectpqiypinylwwmdxirosoplrgkrvytxaoswkeywlixivordrytwlewjyynmysyzensdxeqocozkswnpjejomnlzensdqaphcozxrdjuwtfqhnjyirlrrnfjmvjbsuzsreahvgtqraqhxytxhobq"
    # ciphertext2 = "8b.txt"
    # ciphertext3 = "hdsfgvmkoowafweetcmfthskucaqbilgjofmaqlgspvatvxqbiryscpcfrmvswrvnqlszdmgaoqsakmlupsqforvtwvdfcjzvgsoaoqsacjkbrsevbelvbksarlscdcaarmnvrysywxqgvellcyluwwveoafgclazowafojdlhssfiksepsoywxafowlbfcsocylngqsyzxgjbmlvgrggokgfgmhlmejabsjvgmlnrvqzcrggcrghgeupcyfgtydycjkhqluhgxgzovqswpdvbwsffsenbxapasgazmyuhgsfhmftayjxmwznrsofrsoaopgauaaarmftqsmahvqecev"
    
    # get the passed ciphertext
    ciphertext = sys.argv[1]
    # if the ciphertext is a file, read the file and get the ciphertext
    if ciphertext.startswith('file='):
        with open(ciphertext.split('=')[1],'r') as f:
            ciphertext = f.readlines()
            # get a single string
            ciphertext = ''.join(ciphertext)
    # get the decrypted text, key size and selected keys
    decrypted_text, key_size,selected_keys = _decrypt(ciphertext)
    print("Key Size:",key_size)
    print("Selected Keys:",selected_keys)
    print("Decrypted Text:",decrypted_text)