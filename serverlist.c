#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
//http://slither.io/i33628.txt
//char serverstring[]={"ajickcprzelsarzubipxhlrhgaianpycjqypxszgnjcjpirvghqsbahownvqxelsogvgptefoqbyfmultovcjehetbesxggupwdksjrmtahuicszcqvfbqvubiqhpkryfoiaqxykqqmkuszgofnipwdlyzhvwiookitqxemdlgnubmxxftugmmiimovckbjelszhvvdrsekkgelmtaizhcjqxtmtbrnukmpwlkrygxfahovftqzrqgospdbipwevdyfmtasoxgxslkwbzgnuctbwdkrhumvcsdeeoyyelsarzubiplfkchkpbblsdcjqypxszgnufisfinzzjqdahownvqxeltjgqdglxxhscyfmultovcjraeobejvvdocwdksjrmtahcecmdyhvdgovubiqhpkryfzaalpkovzyktszgofnipwdxwyrqvkmcailqxemdlgnubjzwqotikaygrovckbjelszuzuotyzfqzhomtaizhcjqxnwsepqcikugdkrygxfahovmmqlmwycophkipwevdyfmtjmockuwamncfgnuctbwdkrhlmaewwegnafelsarzubiphjkg"};
char serverstring[]={"ajickcprzelsarzubipxolrhgaianpycjqypxszgnbpjpirvghqscahownvqxelsagvgptefoqbyfmultovcjxzetenrcdmoawdksjrmtahxccrzcqveesnubiqhpkryfmzaqxaotdzotszgofnipwdvvyovwiookisqxemdlgnubixxftugmmigrovckbjelszhovdrsekkggkmtaizhcjqxertbpqciiecjkrygxfahovkmrzplsiknujipwevdyfmteioxgxslkwbzgnuctbwdkrmlmvcsdeeoyyelsarzubipfjkcaqbccmrbcjqypxszgndgiafinzzjqbahownvqxelsjgqdglxxhscyfmultovcjqjeobejvvdocwdksjrmtahwgcmdyhvdgovubiqhpkryfubalbwftbeqmszgofnipwdsdyjnimtxwijqxemdlgnubsqwpotikaygjovckbjelszhbuomrgiywdpmtaizhcjqxftsmkpegwucnkrygxfahovkqqkpuvbmvdkipwevdyfmtonoalmyegqczgnuctbwdkrklmhisuykldgelsarzubipkdkycuucelydcjqypxszgnfcie"};
//char serverstring[]={"ajickcprzelsarzubipwklrhgaianpycjqypxszgnfgjpirvghqsbahownvqxelsegvgptefoqayfmultovcjqaetenrcdmozwdksjrmtahydcrzcqveesnubiqhpkryfpaaqxaotdzotszgofnipwdkwyovymrbxhjqxemdlgnubwuwmtugmmigqovckbjelszlpvdrsekkgepmtaizhcjqxhptbpqciieeikrygxfahovcnrznoaggcahipwevdyfmtmmpxnjqgilshgnuctbwdkratmvnmckolzxelsarzubipwsktctohgsxvcjqypxszgnihiryozaakuuahownvqxelfhgydglxxhozyfmultovcjrheobejvvdocwdksjrmtahaecmdyhvdgovubiqhpkryfuialbwftbeqmszgofnipwdsgyjnimtxwirqxemdlgnubvcwpotikaygjovckbjelszhouomrgiywdpmtaizhcjqxeqsmkpegwucnkrygxfahovmoqkpuvbmvdjipwevdyfmtamoanstzktbignuctbwdkrmomyjkwceoaxelsarzubipemkfhiuacmxhcjqypxszgncmideoqughzcahownvqxelehguyqqyahuzyfmultovcjyceawoowyfsywdksjrmtahwzcy"};
//char serverstring[]={"ajickcprzelsarzubipyqlrhgaianpycjqypxszgnvjjpfeygylnyahownvqxelaihnxqxakgyayfmultovcjqfetenrcdmozwdksjrmtahyjcrzcqvfbqvubiqhpkryfoeaqxaotdgilszgofnipwdkdyovymrbdcsqxemdlgnubqawmtugmmigqovckbjelszjwvdrsekkgepmtaizhcjqxistbpqciieeikrygxfahovdwrzrqgospdbipwevdyfmtapoxgxslkwbzgnuctbwdkrarmvlhoeiuyfelsarzubipygktjfmccrqdcjqypxszgnxeiryrkvfgntahownvqxelafgpwmxyyissyfmultovcjdgewbejvvfmxwdksjrmtahoxcmzchttdoyubiqhpkryfnwakxafrrzkyszgofnipwdxuyizudrzckrqxemdlgnubpbwhxsbpxamiovckbjelszubufjeiptsenmtaizhcjqxpyslkpegwucfkrygxfahovfqqkinceuszlipwevdyfmtbjoiglacsqyjgnuctbwdkrnimglqrxirzgelsarzubipeqkwjopvgpzwcjqypxszgnxeiufgsyakwtahownvqxelelgbcmosefxayfmultovcjyfesakmqcdsvwdksjrmtahwicqummuwdqvubiqhpkryfmwawskksubouszgofnipwdxdyu"};
//char serverstring[]={"alhoegrwgelsarzubipydktlhoegrwgelsarzubipydkt"};
//char serverstring[]={"ahgaianpycjqypxszgnvjjp"};

int main(void)
{
  int16_t *values = malloc(strlen(serverstring)*2);
  uint8_t outData[11];// = malloc(strlen(serverstring)/2);
  int i=1;
  int d=0;
  int offset=0;
  int cnt = 0;
  uint32_t port;
  uint32_t sub;

  offset = 0;
  while(serverstring[offset+1])
  {
    d=0;
    for(i=0;i<22;i++)
    {
  //    printf("%d=",i);
  //    printf("'%c' ",serverstring[offset+i+1]);
      values[i]=serverstring[offset+i+1]-'a';
  //    printf("'%d' ",values[i]);
      sub = 7*(i+offset);
  //    printf("s%d ",sub);
      values[i]=values[i]-sub;
  //    printf("'%d' ",values[i]);  
      values[i]=(values[i]%26+26)%26;
//      printf("'%d(%02X)' ",values[i],values[i]);  

      if((i)%2)
      {
        outData[d]=values[i-1]*16+values[i];
  //      printf("%d %02X",outData[d],outData[d]);
        d++;

      }
  //    printf("\n");
    }
    port = outData[4]*0x10000+outData[5]*0x100+outData[6];
    printf(" %d.%d.%d.%d:%d\n",outData[0],outData[1],outData[2],outData[3],port);
    offset +=22;
  }
  free(values);
  return 0;
}
