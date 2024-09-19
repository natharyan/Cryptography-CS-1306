import numpy as np

with open('7/7.txt','r') as f:
    lines = f.readlines()
    # get a single string
    lines = ''.join(lines)
    # not include any accented characters
    lines = lines.encode('ascii', 'ignore').decode('ascii')
    lines = ''.join([i.lower() for i in lines if i.isalpha()])

def getcountdict(l):
    countdict = {}
    for i in l:
        if i in countdict:
            countdict[i] += 1
        else:
            countdict[i] = 1

    print(len(countdict))
    
    return countdict

def getfreqfict(count_dict, length):
    freqdict = {}
    for char, count in count_dict.items():
        freqdict[ord(char) - ord('a')] = count / length
    return freqdict

def getshifterrors(ref,ciph):
    error_list = []
    error = 0
    for shift in range(26):
        for index in range(26):
            error += np.square(ref[(index+shift)%26] - ciph[index])
        error_list.append(error)
        error = 0
    return error_list

def getplaintext(ciphertext,key):
    plaintext = ''
    for i in ciphertext:
        plaintext += chr((ord(i) - ord('a') - key) % 26 + ord('a'))
    return plaintext

def _getplaintext(ciphtext,ref_freqdict):
    ciph_cntdict = getcountdict(ciphtext)
    ciph_freqdict = getfreqfict(ciph_cntdict,len(ciphtext))
    ciph_freqdict = dict(sorted(ciph_freqdict.items()))
    for i in range(26):
        if i not in ciph_freqdict:
            ciph_freqdict[i] = 0
    shifterrors = getshifterrors(ref_freqdict,ciph_freqdict)
    key = shifterrors.index(min(shifterrors))
    return getplaintext(ciphtext,key)

if __name__ == "__main__":
    ref_cntdict = getcountdict(lines)
    ref_freqdict = getfreqfict(ref_cntdict,len(lines))
    ref_freqdict = dict(sorted(ref_freqdict.items()))

    ciphertext1 = "uryczrvzgenccrqvafvqrnpnrfnepvcurenaqpnagtrgbhg"
    plaintext1 = _getplaintext(ciphertext1,ref_freqdict)
    print(plaintext1)

