# Burrows-Wheeler Transform technique

## intro to BWT

* Used to preprocess strings before actual compression.
* Output with high index of coincidence (IC)
  `==`
* Identical characters are often in groups
* Compression is easy, e.g., with *run-length* encoding.

```
What is the Index of Coincidence?

The coincidence index is an indicator used in cryptanalysis which makes it possible to evaluate the global distribution of letters in encrypted message for a given alphabet.
```

## Ciphering phase (1)

Encode the string `mississippi`, length `N`

Add separator symbol (End Of Line) at the end; must not be present in the string

`S = 'mississippi' + $`

## Ciphering phase (2)

Create all the cyclic rotations/circular shifts of the string `S`

A single character permutation; move the first character to the last place

```
mississippi$
ississippi$m
ssissippi$mi
sissippi$mis
issippi$miss
ssippi$missi
sippi$missis
ippi$mississ
ppi$mississi
pi$mississip
i$mississipp
$mississippi
```

## Ciphering phase (3)

Sort the rotations

```
F          L
$mississippi
i$mississipp
ippi$mississ
issippi$miss
ississippi$m
mississippi$
pi$mississip
ppi$mississi
sippi$missis
sissippi$mis
ssippi$missi
ssissippi$mi
```

F and L are the first and last columns of the matrix

## Ciphering phase (4)

Take only the last column

`L = ipssm$pissii`

and that's the encoded string!

## Deciphering phase (1)

The decipher only sees the encoded string `L`, but

compute the `F` column by sorting `L`

`F = $iiiimppssss`

We want to reconstruct the initial matrix.

## Deciphering phase (2)

Stick together the columns `L F`

They are substrings of the original string `S`, because `L` contains the previous chars of `F`

sort them lexicographically, obtain the first two columns of the original matrix!

## Deciphering phase (3)

Apply `L` in front of the 2 previously discovered columns

Again, by sorting the 3 columns, obtain the first 3 columns of the original matrix

iteratively apply the previous approach --> reconstruct the whole matrix

## Deciphering phase (4)

When the original matrix has been reconstructed, simply search for the row starting with the EOL symbol

This approach is simple, yet..

poor performances:
* `O(N^2)` *space* consumption for storing the `N * N` matrix
* `O(N^2 logN)` *computations*; `N` k-mers sortings with `O(N logN)` sorting algorithm

## Deciphering phase - improvements (1)

The curious *first-last* property
The *relative* order in the group of same letters is preserved

```
F              L
$   mississipp i_1
i_1 $mississip p_1
i_2 ppi$missis s_1
i_3 ssippi$mis s_2
i_4 ssissippi$ m_1
m_1 ississippi $
p_1 i$mississi p_2
p_2 pi$mississ i_2
s_1 ippi$missi s_3
s_2 issippi$mi s_4
s_3 sippi$miss i_3
s_4 sissippi$m i_4
```

Proof:
* consider those rows starting with a specific letter
* delete the first letter, the columns are still sorted
* apply the first letter after `L`, the columns are still sorted, and
* these new rows are available inside the matrix, and they are still sorted relatively to one another

## Deciphering phase - improvements (2)

Start from the EOL symbol `$` in `F`, go backwards into `L`: find `i_1`

`S = '. . . . . . . . . . i'`

## Deciphering phase - improvements (3)

Search for `i_1` in `F`, go backwards in `L` and find `p_1`

`S = '.........pi'`

## Deciphering phase - improvements (4)

Search for `p_1` in `F`, go backwards in `L` and find `p_2`

`S = '........ppi'`

Search for `p_2` in `F`, go backwards in `L` and find `i_2`

`S = '.......ippi'`

## Deciphering phase - improvements (5)

No need to rebuild the original matrix

Linear memory consumption: 2 * `N` --> `O(N)`
Linear number of computations to build the first-last arrays


## Relevant compression techniques (1)

*move-to-front transform* encoding also improves Index Coincidence

can be applied after BWT

using the alphabet a..z, take a string (e.g.,  `ipssm$pissii` ), encode the index of each element

but also modify the alphabet by moving to the front the last char of the alphabet used

There will be lots of low index values


## Relevant compression techniques (2)

An encoded `L` string can be compressed with, e.g., *run-length encoding*.

For long strings, it reduces the memory needed, especially with short alphabets (e.g., DNA strings)

`RLE(L) = RLE(ipssm$pissii) = i1 p1 s2 m1 $1 p1 s2 i2`


## Effectiveness of BWT

normal text contains many occurrences of the word `the`

create the BWT matrix -> lots of rows starting with `he` and ending with `t`


## Comparison with other compression programmes

               Total CPU time/s    Totalcompressed     mean
Programme     compress decompress    size (bytes)    bits/char

compress       9.6 5.2 1246286 3.63
gzip           42.6 4.9 1024887 2.71
Alg-C/D        51.1 9.4 856233 2.43
comp-2         603.2 614.1 848885 2.47


## References

Burrows, Michael; Wheeler, David J. (1994), A block sorting lossless data compression algorithm, Technical Report 124, Digital Equipment Corporation: http://www.hpl.hp.com/techreports/Compaq-DEC/SRC-RR-124.html


# Text Data

A man commented to his lunch companion: My wife had a funny dream last night. She dreamed she'd married a millionaire. You're lucky, sighed the companion. My wife dreams that in the daytime.
ENNTYED_DNEHEEYEEAEOTMESADA__TUD__SDNDYY____HNEEEMPPMLHDNU___AE_EEE____MRHHHHFFRRRRIMHTMII_IIC_TTSTSG_GRWWSNMT_NNLAHC_LI__A__IAM_OOOA__OOAIOU_AAUENTCCCIIIYMMI_DDDRAIM___AHASN___Y_OLLF__KNMM_A

----

http://erdani.com/tdpl/hamlet.txt

Mar. Peace! break thee off! Look where it comes again!
Ber. In the same figure, like the King that's dead.
Mar. Thou art a scholar; speak to it, Horatio.
Ber. Looks it not like the King? Mark it, Horatio.
Hor. Most like. It harrows me with fear and wonder.
Ber. It would be spoke to.
Mar. Question it, Horatio.
Hor. What art thou that usurp'st this time of night
Together with that fair and warlike form
In which the majesty of buried Denmark
Did sometimes march? By heaven I charge thee speak!
Mar. It is offended.
Ber. See, it stalks away!
Hor. Stay! Speak, speak! I charge thee speak!
Mar. 'Tis gone and will not answer.
Ber. How now, Horatio? You tremble and look pale.
Is not this something more than fantasy?
What think you on't?
Hor. Before my God, I might not this believe
Without the sensible and true avouch
Of mine own eyes.
Mar. Is it not like the King?
Hor. As thou art to thyself.
Such was the very armour he had on
When he th' ambitious Norway combated.
So frown'd he once when, in an angry parle,
He smote the sledded Polacks on the ice.
'Tis strange.
Mar. Thus twice before, and jump at this dead hour,
With martial stalk hath he gone by our watch.
Hor. In what particular thought to work I know not;
But, in the gross and scope of my opinion,
This bodes some strange eruption to our state.
Mar. Good now, sit down, and tell me he that knows,
Why this same strict and most observant watch
So nightly toils the subject of the land,
And why such daily cast of brazen cannon
And foreign mart for implements of war;
Why such impress of shipwrights, whose sore task
Does not divide the Sunday from the week.
What might be toward, that this sweaty haste
Doth make the night joint-labourer with the day?
Who is't that can inform me?
Hor. That can I.
At least, the whisper goes so. Our last king,
Whose image even but now appear'd to us,
Was, as you know, by Fortinbras of Norway,
Thereto prick'd on by a most emulate pride,
Dar'd to the combat; in which our valiant Hamlet
(For so this side of our known world esteem'd him)
Did slay this Fortinbras; who, by a seal'd compact,
Well ratified by law and heraldry,
Did forfeit, with his life, all those his lands
Which he stood seiz'd of, to the conqueror;
Against the which a moiety competent
Was gaged by our king; which had return'd
To the inheritance of Fortinbras,
Had he been vanquisher, as, by the same comart
And carriage of the article design'd,
His fell to Hamlet. Now, sir, young Fortinbras,
Of unimproved mettle hot and full,
Hath in the skirts of Norway, here and there,
Shark'd up a list of lawless resolutes,
For food and diet, to some enterprise
That hath a stomach in't; which is no other,
As it doth well appear unto our state,
But to recover of us, by strong hand
And terms compulsatory, those foresaid lands
So by his father lost; and this, I take it,
Is the main motive of our preparations,
The source of this our watch, and the chief head
Of this post-haste and romage in the land.
Ber. I think it be no other but e'en so.
Well may it sort that this portentous figure
Comes armed through our watch, so like the King
That was and is the question of these wars.
Hor. A mote it is to trouble the mind's eye.
In the most high and palmy state of Rome,
A little ere the mightiest Julius fell,
The graves stood tenantless, and the sheeted dead
Did squeak and gibber in the Roman streets;
As stars with trains of fire, and dews of blood,
Disasters in the sun; and the moist star
Upon whose influence Neptune's empire stands
Was sick almost to doomsday with eclipse.
And even the like precurse of fierce events,
As harbingers preceding still the fates
And prologue to the omen coming on,
Have heaven and earth together demonstrated
Unto our climature and countrymen.

108 occurrences of `the`


http://guanine.evolbio.mpg.de/cgi-bin/bwt/bwt.cgi.pl

mlpeeHehhe mmigsdmgrgfmeeeeeeetmierpvtt p sssHHc hcmthllltlrr cv ivfn teleDeMMMMMMMMwlphmwhhmMhpw chtwm m em pwWrWw rrW rtlcehhsthhhhhhhhh hhhhhbwwwwtlttrbfhhHfrrrrrrsmeHeer l lwdmldwtwwdrmmi brmumiu annnn o nirnrnaieutiuiiuaiiiutttr siiai e s eieaienaa'eeeoleen'nennnlnninna'n'annineno'a'nnannoiiinonnnnnne''n'nnnnnno'orenea ns eiii dn no e lnnnrsvthhhhchhrltrlnhuhcnhmhhhlmlggchmskhhhhmvslhsgshrtkhhhhhhhhhrhbctspmdvgcehhnkthhhhhhhHsheebrhmmhhrhrhkkkkdrlktggukbhshchhmc nmtdlrfrrercsyltgkmPdhddurpppppsfpp lwhhr rjrtidigtvmtdlchhhSwtbhriBbrfsesbwWtWfferld vvvzmh'vehmtufDst tmvrNhhpbhvhrhhwdBBBBBBhihh hhhtuttg svtmmmoodvtyrhdrrlli uujlilepggmmremrii d oooooooooeoOoooooooooOOoofol i r fooin ee n nnnnnnnnnnn aAnaaarrnanooiuiiiiuiii ii noiiccctgcttctctctccttcgttcttcttcccc t ccS -WTttTtWwWtttTt ttttttt ttttt Tt ttttttttttttt tttttttT tt tttt tttsWwttttts wtTntwwWwwwc tttsttTttttttttttwWwcttWwwG Tttt ttggggggggTwWWtrtlgsw hhhhhhsrtrtDaDDDsvrrfhftdollthnmnmmnresfflllllllwtoah ltt n a a ttttmm KhmdKkkKKb phhaaotttttttnttlhaspfkhhhhhHThh hhhhhhTh h hDruhlox s erwwwWwwwWblltdeba rsrcalnroaaonaaacraeiaiiiiiaoi s loclllllalllall-ouo u surabbcttbra spwtmmeae r cc uaaeeeieiaeeuaaob iufoitrroeioio o no i ooaeaoa ioa o royeooioo oaa e l s ruooeiiioroe looeaieaeoeewaeaogioeaiioieIIiioeIiIowirwgioooweeueiiiieaoaAaaaaAAaaaaaaaaAaaaAaaaaaaaAaaiaaueoaaaooiuiiiuoiiiiiiiiaaiai uiiiea nk k kkoaioeioaeaaiaeaUueueetStShtsSnnttttttttTtstttthsiiitsi oooooGbDg tTlmtjmoopoPhrsrtrRcccssR cCssccccon ii ipi wggrcimfGttllLLdc FfFHHHHHHHHrHHHHfsmffffwwffspFFFFNNNthhhhhrmmMmmphlhnnnnnnnmmDd mhhyyhYrvrhwyc m hsbithrcnnHnnNnt nrdrnumrme ossssSppsmommsU aa m r miuemisn naiaeueoeuoeuuueeuouuaauuuueeaa
