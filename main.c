/*******************************************************************************************
 *
 *   Margi
 *
 *   Example originally created with raylib 4.5, last time updated with raylib 4.5
 *
 *   Created by Gabriel Rodrigues (@gabriel.grm)
 *
 *   Copyright (c) 2024 @gabriel.grm
 *
 ********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "include/raylib.h"
#include "include/raymath.h"
#include <string.h>
#include <time.h>

const int screenWidth = 1280;
const int screenHeight = 720;
const int velocidade = 5;
const int gravidade = 1;
const int framePulando = 1;
const int frameCaindo = 2;
const int frameParado = 0;
const int plataforma1X = 100;
const int plataformaY = 320;
const int plataforma2X = 830;
const int posicaoYSolo = 0;
bool nuvemEsquerda = true;
bool naPlataforma = false;
typedef struct Contas
{
   char perguntas[10];
   char resposta[10];
   char erradas[10];
   struct Contas *prox;
   struct Contas *ant;
} Contas;

typedef struct Registro{
    char nome[50];
    double tempo;
} Registro;

void inserir(Contas **head, Contas **tail, const char *perguntas, char *resposta, char *errada);
void excluirAtual(Contas **head, Contas **tail, Contas *atual);
void ordenarRanking(const char *nomeArquivo);
void insertionSort(Registro registros[], int n);
int compararTempos(const void *a, const void *b);

int main(void)
{
   clock_t start, end; // para calcular o tempo de jogo
   double tempo;

   FILE *arquivo;

   static int frameContagem = 0;
   static int telaFinalizada = 0;
   static int posicaoLogoX = 0;
   static int posicaoLogoY = 0;
   static int contagemLetras = 0;
   static int topSideRecWidth = 0;
   static int leftSideRecHeight = 0;
   static int bottomSideRecWidth = 0;
   static int rightSideRecHeight = 0;
   static int estado = 0;
   static float alpha = 1.0f;

   char acertosStr[4];
   char nomeRecebido[20] = "";

   int piscando = 0;
   int cinquenta = rand() % 2;
   int tocarsom = 0;
   int acertos = 0;
   int erros = 0;
   int tocarsomInicio = 0;
   int tocarsomBotao = 0;
   int diminuirSom = 0;

   unsigned quantidadeFrames = 3;
   unsigned frameDelay = 5;
   unsigned frameDelayCounter = 0;
   unsigned frameIndex = 0;

   bool respostaCorreta = false;
   bool respostaErrada = false;
   bool cadastro = false;
   bool batendoLado = false;
   bool preenchendoNick = false;
   bool animacao = true;
   bool iniciarTempo = true;
   bool printarArquivo = true;
   bool telaRanking = false;

   ordenarRanking("./include/ranking.txt");

   InitWindow(screenWidth, screenHeight, "margi");
   InitAudioDevice();

   Texture2D personagemTexture = LoadTexture("./resources/textures/personagem.png");
   Texture2D menuTela = LoadTexture("./resources/textures/menuTela.png");
   Texture2D background = LoadTexture("./resources/textures/fundoAzul.png");
   Texture2D cloudTex = LoadTexture("./resources/textures/nuvem.png");
   Texture2D plataforma = LoadTexture("./resources/textures/plataforma.png");
   Texture2D vidaCheia = LoadTexture("./resources/textures/vidaCheia.png");
   Texture2D vidaVazia = LoadTexture("./resources/textures/vidaVazia.png");
   Texture2D trofeu = LoadTexture("./resources/textures/trofeu.png");
   Texture2D seta = LoadTexture("./resources/textures/seta.png");
   Texture2D inserirNome = LoadTexture("./resources/textures/digitarNick.png");
   Texture2D piso = LoadTexture("./resources/textures/piso.png");
   Texture2D leaderboard = LoadTexture("./resources/textures/ranks.png");
   Texture2D relogio = LoadTexture("./resources/textures/relogio.png");

   Image iconePersonagem = LoadImage("./resources/textures/personagemico.png");
   SetWindowIcon(iconePersonagem);

   Sound pop = LoadSound("./resources/audio/transicao.mp3");
   Sound somAlternativaCorreta = LoadSound("./resources/audio/alternativaCorreta.wav");
   Sound somAlternativaErrada = LoadSound("./resources/audio/alternativaErrada.mp3");
   Sound somClickBotao = LoadSound("./resources/audio/clickBotao.mp3");
   Sound somMusicaMenu = LoadSound("./resources/audio/musicaMenu.mp3");
   Sound somPulo = LoadSound("./resources/audio/salto.mp3");
   SetSoundVolume(somPulo, 0.10f);
   SetSoundVolume(somMusicaMenu, 0.20f);
   SetSoundVolume(somClickBotao, 1.0f);

   int frameWidth = personagemTexture.width / quantidadeFrames;
   posicaoLogoX = GetScreenWidth() / 2 - 128;
   posicaoLogoY = GetScreenHeight() / 2 - 128;

   Rectangle frameRec = {0.0f, 0.0f, (float)frameWidth, (float)personagemTexture.height};
   Rectangle plataforma1 = {plataforma1X, plataformaY, 350, 80};
   Rectangle plataforma2 = {plataforma2X, plataformaY, 350, 80};
   Rectangle floor = {0, 580, 1280, 140};
   Rectangle alternativa1 = {255, 250, 80, 80};
   Rectangle alternativa2 = {985, 250, 80, 80};
   
   unsigned posicaoYSolo = 580 - personagemTexture.height;

   Vector2 posicaoPersonagem = {580, screenHeight / 2.0f};
   Vector2 velocidadePersonagem = {0.0f, 0.0f};
   Vector2 movimentoNuvem = {0.0f, 0.0f};

   SetTargetFPS(60);
   
   Contas *head1 = NULL, *tail1 = NULL;
   Contas *head2 = NULL, *tail2 = NULL;
   Contas *head3 = NULL, *tail3 = NULL;
   bool telaMenu = false;

   // Primeiro Nível - Fácil
   inserir(&head1, &tail1, "6*0", "0", "6");
   inserir(&head1, &tail1, "7+9", "16", "15");
   inserir(&head1, &tail1, "8+13", "21", "24");
   inserir(&head1, &tail1, "3+7", "10", "14");
   inserir(&head1, &tail1, "4+3", "7", "12");
   inserir(&head1, &tail1, "5-2", "3", "4");
   inserir(&head1, &tail1, "6-1", "5", "4");
   inserir(&head1, &tail1, "3*9", "27", "18");
   inserir(&head1, &tail1, "3*2", "6", "8");
   inserir(&head1, &tail1, "9+8", "17", "18");
   inserir(&head1, &tail1, "4+5", "9", "10");
   inserir(&head1, &tail1, "12*2", "24", "22");
   inserir(&head1, &tail1, "9*1", "9", "8");
   inserir(&head1, &tail1, "5*2", "10", "11");
   inserir(&head1, &tail1, "1+9", "10", "11");
   inserir(&head1, &tail1, "6+3", "9", "10");
   inserir(&head1, &tail1, "7-4", "3", "2");
   inserir(&head1, &tail1, "8-3", "5", "6");
   inserir(&head1, &tail1, "9-2", "7", "8");
   inserir(&head1, &tail1, "10-1", "9", "8");

   // Segundo Nível - Médio
   inserir(&head2, &tail2, "12+15", "27", "25");
   inserir(&head2, &tail2, "14-6", "8", "9");
   inserir(&head2, &tail2, "18/2", "9", "8");
   inserir(&head2, &tail2, "16*2", "32", "34");
   inserir(&head2, &tail2, "25/5", "5", "4");
   inserir(&head2, &tail2, "27+9", "36", "34");
   inserir(&head2, &tail2, "35-9", "26", "27");
   inserir(&head2, &tail2, "22*2", "44", "46");
   inserir(&head2, &tail2, "45/9", "5", "6");
   inserir(&head2, &tail2, "12*4", "48", "50");
   inserir(&head2, &tail2, "36/6", "6", "7");
   inserir(&head2, &tail2, "23-5", "18", "17");
   inserir(&head2, &tail2, "20/4", "5", "6");
   inserir(&head2, &tail2, "36/3", "12", "9");
   inserir(&head2, &tail2, "42-17", "25", "29");
   inserir(&head2, &tail2, "60/12", "5", "4");
   inserir(&head2, &tail2, "63+15", "78", "74");
   inserir(&head2, &tail2, "53-20", "33", "31");
   inserir(&head2, &tail2, "15*3", "45", "44");
   inserir(&head2, &tail2, "32+17", "49", "51");

   // Terceiro Nível - Difícil
   inserir(&head3, &tail3, "72/8", "9", "10");
   inserir(&head3, &tail3, "70*70", "1400", "140");
   inserir(&head3, &tail3, "127-42", "85", "75");
   inserir(&head3, &tail3, "9*12", "108", "99");
   inserir(&head3, &tail3, "65*3", "195", "196");
   inserir(&head3, &tail3, "12*11", "132", "144");
   inserir(&head3, &tail3, "44*1.5", "66", "78");
   inserir(&head3, &tail3, "29*3", "87", "97");
   inserir(&head3, &tail3, "68*1.5", "102", "92");
   inserir(&head3, &tail3, "102-37", "65", "75");
   inserir(&head3, &tail3, "12/1.5", "8", "10");
   inserir(&head3, &tail3, "12^0", "1", "12");
   inserir(&head3, &tail3, "0!", "1", "0");
   inserir(&head3, &tail3, "203-18", "185", "175");
   inserir(&head3, &tail3, "183/3", "61", "63");
   inserir(&head3, &tail3, "15*17", "255", "240");
   inserir(&head3, &tail3, "225/5", "45", "44");
   inserir(&head3, &tail3, "24*8", "192", "210");
   inserir(&head3, &tail3, "255+45", "300", "299");
   inserir(&head3, &tail3, "193-25", "168", "158");

   // sorteando em qual pergunta começar
   for (int i = 0; i < rand() % 20; i++)
   {
      head1 = head1->prox;
   }
   for (int i = 0; i < rand() % 20; i++)
   {
      head2 = head2->prox;
   }
   for (int i = 0; i < rand() % 20; i++)
   {
      head3 = head3->prox;
   }

   topSideRecWidth = 16;
   leftSideRecHeight = 16;
   bottomSideRecWidth = 16;
   rightSideRecHeight = 16;
   
   estado = 0;
   alpha = 1.0f;

   while (!WindowShouldClose()) // detecta se a janela foi fechada com o X ou o ESC
   {
      Rectangle startGame = {489, 343, 277, 37};
      Rectangle ranks = {576, 395, 124, 37};
      Rectangle exit = {590, 445, 97, 36};
      Rectangle foot = {posicaoPersonagem.x, posicaoPersonagem.y + 110, personagemTexture.width / 3, personagemTexture.height - 100};
      Rectangle head = {posicaoPersonagem.x, posicaoPersonagem.y + 10, personagemTexture.width / 3, personagemTexture.height - 100};
      Rectangle lado1 = {posicaoPersonagem.x, posicaoPersonagem.y + 20, personagemTexture.width / 9, personagemTexture.height - 30};
      Rectangle lado2 = {posicaoPersonagem.x + 70, posicaoPersonagem.y + 20, personagemTexture.width / 9, personagemTexture.height - 30};
      Rectangle nickname = {502, 363, 277, 38};

      bool plataform1Collision = CheckCollisionRecs(foot, plataforma1);
      bool plataform2Collision = CheckCollisionRecs(foot, plataforma2);
      bool plataform1CollisionHead = CheckCollisionRecs(head, plataforma1);
      bool plataform2CollisionHead = CheckCollisionRecs(head, plataforma2);
      bool OnFloor = CheckCollisionRecs(foot, floor);
      bool alternativa1Collision = CheckCollisionRecs(foot, alternativa1);
      bool alternativa2Collision = CheckCollisionRecs(foot, alternativa2);

      //----------------------------------------------------------------------------------

      if (nuvemEsquerda)
      {
         movimentoNuvem.x -= 0.1f;
         if (movimentoNuvem.x <= -40.0f)
         {
            nuvemEsquerda = false;
         }
      }
      else
      {
         movimentoNuvem.x += 0.1f;
         if (movimentoNuvem.x >= 0)
         {
            nuvemEsquerda = true;
         }
      }
      if (estado == 0)
      {
         frameContagem++;

         if (frameContagem == 80)
         {
            estado = 1;
            frameContagem = 0;
         }
      }
      else if (estado == 1)
      {
         topSideRecWidth += 8;
         leftSideRecHeight += 8;

         if (topSideRecWidth == 256)
            estado = 2;
      }
      else if (estado == 2)
      {
         bottomSideRecWidth += 8;
         rightSideRecHeight += 8;

         if (bottomSideRecWidth == 256)
            estado = 3;
      }
      else if (estado == 3)
      {
         frameContagem++;

         if (contagemLetras < 10)
         {
            if (frameContagem / 12)
            {
               contagemLetras++;
               frameContagem = 0;
            }
         }
         else
         {
            if (frameContagem > 200)
            {
               alpha -= 0.02f;

               if (alpha <= 0.0f)
               {
                  alpha = 0.0f;
                  if (telaFinalizada == 0)
                  {
                     telaFinalizada = 1;
                     telaMenu = true;
                  }
               }
            }
         }
      }
      if (plataform1Collision || plataform2Collision)
      {
         naPlataforma = true;
         velocidadePersonagem.y = 0;
      }
      if (naPlataforma && !plataform1Collision && !plataform2Collision)
      {
         naPlataforma = false;
      }
      if (plataform1CollisionHead || plataform2CollisionHead)
      {
         velocidadePersonagem.y = 0;
         velocidadePersonagem.y += gravidade;
      }
      if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_W)) && (posicaoPersonagem.y + personagemTexture.height == 580 || naPlataforma) && !batendoLado && cadastro)
      {
         PlaySound(somPulo);
         velocidadePersonagem.y = -6 * velocidade;
      }
      if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && !batendoLado && cadastro)
      {
         velocidadePersonagem.x = velocidade;
         if (frameRec.width < 0)
         {
            frameRec.width = -frameRec.width;
         }
      }
      else if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && !batendoLado && cadastro)
      {
         velocidadePersonagem.x = -velocidade;
         if (frameRec.width > 0)
         {
            frameRec.width = -frameRec.width;
         }
      }
      else
      {
         velocidadePersonagem.x = 0;
      }

      bool personagemAndando = velocidadePersonagem.x != 0 || velocidadePersonagem.y != 0;
      posicaoPersonagem = Vector2Add(posicaoPersonagem, velocidadePersonagem);

      if (posicaoPersonagem.y >= 580 - personagemTexture.height)
      {
         velocidadePersonagem.y = 0;
         posicaoPersonagem.y = posicaoYSolo;
      }
      else if (!naPlataforma)
      {
         velocidadePersonagem.y += gravidade;
      }

      ++frameDelayCounter;
      if (frameDelayCounter >= frameDelay)
      {
         frameDelayCounter = 0;
         if (personagemAndando)
         {
            if (naPlataforma || OnFloor)
            {
               ++frameIndex;
               frameIndex %= quantidadeFrames;
               if (frameIndex == 0)
               {
                  frameIndex = 1;
               }
            }
            else
            {
               if (velocidadePersonagem.y < 0)
               {
                  frameIndex = framePulando;
               }
               else
               {
                  frameIndex = frameCaindo;
               }
            }
            frameRec.x = frameIndex * (float)frameWidth;
         }
         else
         {
            frameRec.x = frameParado;
         }
      }
      if (cinquenta == 0 && alternativa1Collision)
      { // resposta na esquerda
         respostaCorreta = true;
         tocarsom++;
      }
      else if (cinquenta == 0 && alternativa2Collision)
      { // resposta na direita
         respostaErrada = true;
         tocarsom++;
      }
      else if (cinquenta == 1 && alternativa1Collision)
      { // resposta na esquerda
         respostaErrada = true;
         tocarsom++;
      }
      else if (cinquenta == 1 && alternativa2Collision)
      { // resposta na direita
         respostaCorreta = true;
         tocarsom++;
      }
      else
      {
         respostaCorreta = false;
         respostaErrada = false;
      }
      if (CheckCollisionRecs(lado1, plataforma1) || CheckCollisionRecs(lado1, plataforma2))
      {
         velocidadePersonagem.x = 0;
         posicaoPersonagem.x = posicaoPersonagem.x + 5;
         velocidadePersonagem.y += gravidade;
         batendoLado = true;
      }
      else if (CheckCollisionRecs(lado2, plataforma1) || CheckCollisionRecs(lado2, plataforma2))
      {
         velocidadePersonagem.x = 0;
         posicaoPersonagem.x = posicaoPersonagem.x - 20;
         velocidadePersonagem.y += gravidade;
         batendoLado = true;
      }
      else if (CheckCollisionRecs(foot, floor))
      {
         batendoLado = false;
      }

      //----------------------------------------------------------------------------------
      // Desenho
      //----------------------------------------------------------------------------------

      BeginDrawing();
      ClearBackground(RAYWHITE);
      if (estado == 0)
      {
         if ((frameContagem / 10) % 2)
            DrawRectangle(posicaoLogoX, posicaoLogoY, 16, 16, BLACK);
      }
      else if (estado == 1)
      {
         DrawRectangle(posicaoLogoX, posicaoLogoY, topSideRecWidth, 16, BLACK);
         DrawRectangle(posicaoLogoX, posicaoLogoY, 16, leftSideRecHeight, BLACK);
      }
      else if (estado == 2)
      {
         DrawRectangle(posicaoLogoX, posicaoLogoY, topSideRecWidth, 16, BLACK);
         DrawRectangle(posicaoLogoX, posicaoLogoY, 16, leftSideRecHeight, BLACK);

         DrawRectangle(posicaoLogoX + 240, posicaoLogoY, 16, rightSideRecHeight, BLACK);
         DrawRectangle(posicaoLogoX, posicaoLogoY + 240, bottomSideRecWidth, 16, BLACK);
      }
      else if (estado == 3)
      {
         DrawRectangle(posicaoLogoX, posicaoLogoY, topSideRecWidth, 16, Fade(BLACK, alpha));
         DrawRectangle(posicaoLogoX, posicaoLogoY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

         DrawRectangle(posicaoLogoX + 240, posicaoLogoY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
         DrawRectangle(posicaoLogoX, posicaoLogoY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

         DrawRectangle(GetScreenWidth() / 2 - 112, GetScreenHeight() / 2 - 112, 224, 224, Fade(RAYWHITE, alpha));

         DrawText(TextSubtext("gabrielgrm", 0, contagemLetras), GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 48, 39, Fade(BLACK, alpha));

         if (frameContagem > 20)
            DrawText("powered by", posicaoLogoX, posicaoLogoY - 27, 20, Fade(DARKGRAY, alpha));
      }
      if (telaMenu && telaFinalizada == 1 && !telaRanking)
      {
         if (tocarsomInicio == 0)
         {
            PlaySound(somMusicaMenu);
            tocarsomInicio = 1;
         }
         ClearBackground(RAYWHITE);
         DrawTexture(menuTela, 0, 0, WHITE);
         if (CheckCollisionPointRec(GetMousePosition(), startGame))
         {
            DrawTexture(seta, 459, 343, WHITE);
         }
         else if (CheckCollisionPointRec(GetMousePosition(), ranks))
         {

            DrawTexture(seta, 546, 395, WHITE);
         }
         else if (CheckCollisionPointRec(GetMousePosition(), exit))
         {

            DrawTexture(seta, 560, 445, WHITE);
         }
         else
         {
            tocarsomBotao = 0;
         }
      }
      if ((CheckCollisionPointRec(GetMousePosition(), startGame) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && telaFinalizada == 1) || (!telaMenu && telaFinalizada == 1 && !telaRanking))
      {
         if (tocarsomBotao == 0)
            {
               PlaySound(somClickBotao);
               tocarsomBotao = 1;
            }
         telaMenu = false;
                     
               if (!cadastro){
               if (diminuirSom == 0)
               {
                  SetSoundVolume(somMusicaMenu, 0.20f);
               }
               else if (diminuirSom <= 10)
               {
                  SetSoundVolume(somMusicaMenu, 0.17f);
               }
               else if (diminuirSom <= 20)
               {
                  SetSoundVolume(somMusicaMenu, 0.15f);
               }
               else if (diminuirSom <= 30)
               {
                  SetSoundVolume(somMusicaMenu, 0.13f);
               }
               else if (diminuirSom <= 40)
               {
                  SetSoundVolume(somMusicaMenu, 0.11f);
               }
               else if (diminuirSom <= 50)
               {
                  SetSoundVolume(somMusicaMenu, 0.09f);
               }
               else if (diminuirSom <= 60)
               {
                  SetSoundVolume(somMusicaMenu, 0.07f);
               }
               else if (diminuirSom <= 70)
               {
                  SetSoundVolume(somMusicaMenu, 0.05f);
               }
               else if (diminuirSom <= 80)
               {
                  SetSoundVolume(somMusicaMenu, 0.03f);
               }
               else if (diminuirSom <= 90)
               {
                  SetSoundVolume(somMusicaMenu, 0.01f);
               }
               else
               {
                  StopSound(somMusicaMenu);
               }

               diminuirSom++;

            ClearBackground(RAYWHITE);
            DrawTexture(inserirNome, 0, 0, WHITE);
            DrawText("Digite seu nome:", 515, 315, 30, BLACK);
            DrawText(nomeRecebido, 520, 371, 25, (Color) {80,37,29,255});
            DrawText(nomeRecebido, 520, 369, 25, WHITE);

         if (CheckCollisionPointRec(GetMousePosition(), nickname)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
               preenchendoNick = true;
            }
               if (preenchendoNick) {
               char letra = GetKeyPressed();
               if (strlen(nomeRecebido) > 13)  {
                  nomeRecebido[strlen(nomeRecebido) - 1] = '\0';
               }
               if (IsKeyPressed(KEY_BACKSPACE)) {
                  nomeRecebido[strlen(nomeRecebido) - 1] = '\0';
               }
               if (letra >= 32 && letra <= 126) {
                  nomeRecebido[strlen(nomeRecebido)] = letra;
               } else if (letra == 8) {
                  nomeRecebido[strlen(nomeRecebido) - 1] = '\0';
               }
               if (IsKeyPressed(KEY_ENTER)) {
                  cadastro = true;
                  preenchendoNick = false;
               }
            } else {
               if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                  preenchendoNick = false;
               }
            }
         } else {
            preenchendoNick = false;
         }
            }
         if(cadastro && animacao) {
            DrawTexture(piso, 0, 580, Fade(WHITE, 0.5f));
            PlaySound(pop);
            animacao = false;
         }
         if (cadastro && !animacao)
         {
            if(iniciarTempo) {
               start = clock();
               iniciarTempo = false;
            }
            ClearBackground(RAYWHITE);
            if (acertos <= 20 && erros < 2)
            {
               DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 2.3f, WHITE);
               DrawTextureEx(cloudTex, movimentoNuvem, 0.0f, 2.3f, WHITE);
               DrawTexture(piso, 0, 580, WHITE);
               if (acertos <= 5)
               {
                  DrawText(head1->perguntas, 604, 644, 80, (Color){63, 40, 19, 255});
                  DrawText(head1->perguntas, 600, 640, 80, (Color){130, 127, 32, 255});
               }
               else if (acertos <= 10)
               {
                  DrawText(head2->perguntas, 604, 644, 80, (Color){63, 40, 19, 255});
                  DrawText(head2->perguntas, 600, 640, 80, (Color){130, 127, 32, 255});
               }
               else
               {
                  DrawText(head3->perguntas, 604, 644, 80, (Color){63, 40, 19, 255});
                  DrawText(head3->perguntas, 600, 640, 80, (Color){130, 127, 32, 255});
               }

               if (cinquenta == 0)
               {
                  if (acertos <= 5)
                  {
                     DrawText(head1->resposta, 277, 252, 80, GRAY);
                     DrawText(head1->resposta, 275, 250, 80, BLACK);
                     DrawText(head1->erradas, 1007, 252, 80, GRAY);
                     DrawText(head1->erradas, 1005, 252, 80, BLACK);
                  }
                  else if (acertos <= 10)
                  {
                     DrawText(head2->resposta, 277, 252, 80, GRAY);
                     DrawText(head2->resposta, 275, 250, 80, BLACK);
                     DrawText(head2->erradas, 1007, 252, 80, GRAY);
                     DrawText(head2->erradas, 1005, 252, 80, BLACK);
                  }
                  else
                  {
                     DrawText(head3->resposta, 277, 252, 80, GRAY);
                     DrawText(head3->resposta, 275, 250, 80, BLACK);
                     DrawText(head3->erradas, 1007, 252, 80, GRAY);
                     DrawText(head3->erradas, 1005, 252, 80, BLACK);
                  }
               }
               else
               {
                  if (acertos <= 5)
                  {
                     DrawText(head1->erradas, 277, 252, 80, GRAY);
                     DrawText(head1->erradas, 275, 250, 80, BLACK);
                     DrawText(head1->resposta, 1007, 252, 80, GRAY);
                     DrawText(head1->resposta, 1005, 252, 80, BLACK);
                  }
                  else if (acertos <= 10)
                  {
                     DrawText(head2->erradas, 277, 252, 80, GRAY);
                     DrawText(head2->erradas, 275, 250, 80, BLACK);
                     DrawText(head2->resposta, 1007, 252, 80, GRAY);
                     DrawText(head2->resposta, 1005, 252, 80, BLACK);
                  }
                  else
                  {
                     DrawText(head3->erradas, 277, 252, 80, GRAY);
                     DrawText(head3->erradas, 275, 250, 80, BLACK);
                     DrawText(head3->resposta, 1007, 252, 80, GRAY);
                     DrawText(head3->resposta, 1005, 252, 80, BLACK);
                  }
               }

               DrawTextureRec(personagemTexture, frameRec, posicaoPersonagem, WHITE);
               DrawTexture(plataforma, 100, 320, WHITE);
               DrawTexture(plataforma, 830, 320, WHITE);
               itoa(acertos, acertosStr, 10);
               DrawText(acertosStr, 10, 10, 40, BLACK);
               if (respostaCorreta)
               {
                  DrawText("RESPOSTA CORRETA", 360, 80, 40, GREEN);
                  if (tocarsom == 1)
                  {
                     PlaySound(somAlternativaCorreta);
                     if (acertos <= 5)
                     {
                        excluirAtual(&head1, &tail1, head1);
                        head1 = head1->prox;
                     }
                     else if (acertos <= 10)
                     {
                        excluirAtual(&head2, &tail2, head2);
                        head2 = head2->prox;
                     }
                     else if (acertos <= 20)
                     {
                        excluirAtual(&head3->ant, &tail3, head3->ant);
                        head3 = head3->prox;
                     }
                     if (acertos <= 20)
                     {
                        posicaoPersonagem = (Vector2){posicaoYSolo + 100, screenHeight / 2.0f};
                     }
                     tocarsom = 0;
                     cinquenta = rand() % 2;
                     acertos++;
                  }
               }
               if (erros == 0)
               {
                  DrawTexture(vidaCheia, 1225, 10, WHITE);
                  DrawTexture(vidaCheia, 1250, 10, WHITE);
               }
               else if (erros == 1)
               {
                  DrawTexture(vidaVazia, 1225, 10, WHITE);
                  DrawTexture(vidaCheia, 1250, 10, WHITE);
               }
               else
               {
                  DrawTexture(vidaVazia, 1225, 10, WHITE);
                  DrawTexture(vidaVazia, 1250, 10, WHITE);
               }
               if (respostaErrada)
               {
                  DrawText("RESPOSTA ERRADA", 360, 80, 40, RED);
                  if (tocarsom == 1)
                  {
                     PlaySound(somAlternativaErrada);
                     posicaoPersonagem = (Vector2){posicaoYSolo + 100, screenHeight / 2.0f};
                     tocarsom = 0;
                     cinquenta = rand() % 2;
                     erros++;
                  }
               }
            }
            else if (acertos > 20)
            {
               if (printarArquivo) {
               end = clock();
               tempo = (double)(end - start) / CLOCKS_PER_SEC;
               arquivo = fopen("./include/ranking.txt", "a");
               fprintf(arquivo, "%s %f\n", nomeRecebido,tempo);
               printarArquivo = false;
               }
               ClearBackground(RAYWHITE);
               DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 2.3f, WHITE);
               DrawTextureEx(cloudTex, movimentoNuvem, 0.0f, 2.3f, WHITE);
               DrawTexture(piso, 0, 580, WHITE);
               DrawTextureRec(personagemTexture, frameRec, posicaoPersonagem, WHITE);
               DrawTexture(plataforma, 100, 320, WHITE);
               DrawTexture(plataforma, 830, 320, WHITE);
               DrawTexture(trofeu, 556, 412, WHITE);
               DrawText("PARABENS, VOCE VENCEU!", 369, 104, 40, DARKBLUE);
               DrawText("PARABENS, VOCE VENCEU!", 365, 100, 40, WHITE);
            }
            else
            {
               ClearBackground(RAYWHITE);
               DrawTextureEx(background, (Vector2){0, 0}, 0.0f, 2.3f, WHITE);
               DrawTextureEx(cloudTex, movimentoNuvem, 0.0f, 2.3f, WHITE);
               DrawTexture(piso, 0, 580, WHITE);
               DrawTextureRec(personagemTexture, frameRec, posicaoPersonagem, WHITE);
               DrawTexture(plataforma, 100, 320, WHITE);
               DrawTexture(plataforma, 830, 320, WHITE);

               DrawText("VOCE PERDEU!", 384, 84, 60, RED);
               DrawText("VOCE PERDEU!", 380, 80, 60, WHITE);
               if (piscando >= 1 && piscando <= 20)
               {
                  DrawText("Pressione ESC para sair", 540, 150, 10, BLACK);
               }
               piscando++;
               if (piscando == 60)
               {
                  piscando = 0;
               }
            }
         }
      }
      else if ((CheckCollisionPointRec(GetMousePosition(), ranks) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && telaFinalizada == 1) || (telaRanking && telaFinalizada == 1))
      {
         telaRanking = true;
         if (tocarsomBotao == 0)
            {
               PlaySound(somClickBotao);
               tocarsomBotao = 1;
            }
         Rectangle sairX = {1027, 138, 34, 34};
         ClearBackground(RAYWHITE);
         DrawTexture(leaderboard, 0, 0, WHITE);
         FILE *arquivo;
         arquivo = fopen("./include/ranking.txt", "r");
         char nome[20];
         double tempo;
         int minutos;
         int segundos;
         char tempoStr[10];
         char minutosStr[5];
         char segundosStr[5];
         int i = 0;
         while (fscanf(arquivo, "%s %lf", nome, &tempo) != EOF)
         {
            if (i < 5) {
               minutos = (int)tempo / 60;
               segundos = (int)tempo % 60;
               itoa(minutos, minutosStr, 10);
               itoa(segundos, segundosStr, 10);
               strcpy(tempoStr, minutosStr);
               strcat(tempoStr, "m");
               strcat(tempoStr, segundosStr);
               strcat(tempoStr, "s");
               DrawText(nome, 280, 218 + i * 58, 30, (Color){215, 147, 34, 255});//144, 37, 29, 255
               DrawText(nome, 280, 215 + i * 58, 30, BLACK);
               DrawText(tempoStr, 900, 218 + i * 58, 30, (Color) {215, 147, 34, 255});
               DrawText(tempoStr, 900, 215 + i * 58, 30, BLACK);
               DrawTexture(relogio, 865, 210 + i * 58, WHITE);
            }
            i++;
         }
         fclose(arquivo);
         if (CheckCollisionPointRec(GetMousePosition(), sairX) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && telaFinalizada == 1)
         {
            PlaySound(somClickBotao);
            telaRanking = false;
         }
      }
      else if (CheckCollisionPointRec(GetMousePosition(), exit) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && telaFinalizada == 1)
      {
         if (tocarsomBotao == 0)
            {
               PlaySound(somClickBotao);
               tocarsomBotao = 1;
            }
         break;
      }
      EndDrawing();
   }
   CloseAudioDevice();

   CloseWindow();

   return 0;
}

void inserir(Contas **head, Contas **tail, const char *perguntas, char *resposta, char *errada)
{
   Contas *novo = (Contas *)malloc(sizeof(Contas));
   strcpy(novo->perguntas, perguntas);
   strcpy(novo->resposta, resposta);
   strcpy(novo->erradas, errada);
   novo->prox = *head;
   novo->ant = NULL;
   if (*head == NULL)
   {
      *head = novo;
      *tail = novo;
   }
   else
   {
      (*tail)->prox = novo;
      novo->ant = *tail;
      *tail = novo;
   }
}

void excluirAtual(Contas **head, Contas **tail, Contas *atual)
{
   if (atual == NULL)
   {
      return;
   }
   if (*head == *tail && atual == *head)
   {
      free(atual);
      *head = NULL;
      *tail = NULL;
      return;
   }

   if (atual == *head)
   {
      *head = atual->prox;
      if (*head != NULL)
      {
         (*head)->ant = NULL;
      }
      free(atual);
      return;
   }

   if (atual == *tail)
   {
      *tail = atual->ant;
      (*tail)->prox = NULL;
      free(atual);
      return;
   }

   atual->ant->prox = atual->prox;
   atual->prox->ant = atual->ant;
   free(atual);
}

void insertionSort(Registro registros[], int n) {
    int i, j;
    Registro chave;
    for (i = 1; i < n; i++) {
        chave = registros[i];
        j = i - 1;

        while (j >= 0 && registros[j].tempo > chave.tempo) {
            registros[j + 1] = registros[j];
            j = j - 1;
        }
        registros[j + 1] = chave;
    }
}

void ordenarRanking(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para leitura");
        return;
    }

    Registro registros[100];
    int numRegistros = 0;

    while (fscanf(arquivo, "%s %lf", registros[numRegistros].nome, &registros[numRegistros].tempo) == 2) {
        numRegistros++;
        if (numRegistros >= 100) break;
    }
    fclose(arquivo);

    insertionSort(registros, numRegistros);

    arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }

    for (int i = 0; i < numRegistros; i++) {
        fprintf(arquivo, "%s %.6lf\n", registros[i].nome, registros[i].tempo);
    }
    fclose(arquivo);
}