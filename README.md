## Compilation
1. Client\
GUI se trouve dans le dossier ./src/byterun/client, vous pouvez utiliser `make` pour compiler le programme de client. Par contre le fichier exécutable `gui` est bougé au dossier `/tmp/`. Dans le fonction `simul_init` (se trouve dans `./src/byterun/simul/sf-regs.c`), le processus fils exécute `/tmp/gui` pour exécuter le client.
```
  cd ./src/byterun/simul/client
  make
```
2. Server\
Compiler `./mytest/test.ml/` par `make` et exécuter `test.elf`


## Processus du projet
On a changé la mécanisme de synchronization dans la version actuelle. On a utilisé shared memory(`shmget, shmat, shmdt, shmctl`) pour IPC (communication entre processus) ainsi que la condition variable (`pthread_cond_t`) et le mutex (`pthread_mutex_t`) pour protéger les données partagées.

#### variable utilisé
dans le serveur (`sf-regs.c`):\
`shw` est le mémoire partagé par le client écouteur et le serveur écriture.\
`shr` est le mémoire partagé par le serveur écouteur et le serveur écriture.\
dans le client (`gui.c`):\
`shr` est le mémoire partagé par le client écouteur et le serveur écriture.\
`shw` est le mémoire partagé par le serveur écouteur et le serveur écriture.\
`struct shared_use_st` est défini dans le fichier `./src/byterun/simul/shard.h`.
```c
#define TEXT_SZ 2048
struct shared_use_st{
  int shmid; //shared memory's id
  // pour vérifier s'il y a msg qui est écrit dans le mémoire
  int written;
  pthread_mutex_t mute;
  pthread_cond_t cond_r;
  pthread_cond_t cond_w;
  char text[TEXT_SZ];
};
```


![graph](https://github.com/XIANQw/OMicroB/blob/microbit/doc/graph.jpg)

#### Synchronization entre serveur écriture et client écouteur.
La tâche du serveur écriture:
1. Demander le mutex.
2. Vérifier la variable partagé `written`,
  - Si 1, ça veut dire que la dernière instruction n'est pas traité par le client écouteur, le serveur écriture se bloque.
  - Si 0, ça veut dire que le serveur peut envoyer l'instruction suivante.
3. Ecrire une instruction dans le mémoire partagé.
4. Réveiller le client. (On est sûr que le client se bloque initialement, parceque le processus de client exécute avant de serveur).
5. Rendre le mutex.

La tâche du client écouteur:
1. Demander le mutex.
2. Vérifier la variable partagé `written`,
  - Si 0, ça veut dire que le serveur n'a pas envoyé la nouvelle instruction, le client écouteur se bloque.
  - Si 1, ça veut dire que la nouvelle instruction vient, il faut le traiter.
3. Traiter l'instruction
4. Réveiller le serveur écriture.
5. Rendre le mutex.

#### Synchronization entre serveur écouteur et client écriture.
La tâche du client écriture:
1. Demander le mutex.
2. Vérifier la variable partagé `written`.
  - Si 1, ça veut dire que la dernière instruction n'est pas traité par le serveur écouteur, le client écriture se bloque.
  - Si 0, ça veut dire que le client peut envoyer l'instruction suivante.
3. Ecrire une instruction dans le mémoire partagé.
4. Réveiller le serveur.
5. Rendre le mutex.

La tâche du serveur écouteur:
1. Demander le mutex.
2. Vérifier la variable partagé `written`.
  - Si 0, ça veut dire que le client n'a pas envoyé la nouvelle instruction, le serveur écouteur se bloque.
  - Si 1, ça veut dire que la nouvelle instruction vient, il faut le traiter.
3. Ecrire une instruction dans le mémoire partagé.
4. Réveiller le serveur.
5. Rendre le mutex.

#### Problème à résoudre
`pthread_cond_signal` ne fonctionne pas, l'image suivant est le résultat d'exécution. On peut voir que client écouteur se bloque initialement, ensuite le serveur exécute et il écrit "OMicroB!" dans le mémoire partagé, il réveille le client écouteur en utilisant `pthread_cond_signal`, et il n'y a aucune d'erreur. (S'il y a des erreur, `pthread_cond_signal` rend un int négatif).
Dans le prochain tour de bloc, le serveur se bloque aussi, donc dead lock se produit.

![résultat](https://github.com/XIANQw/OMicroB/blob/microbit/doc/resultat.png)
