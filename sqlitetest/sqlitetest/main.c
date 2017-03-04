//
//  main.c
//  sqlitetest
//
//  Created by Haijin Yan on 1/7/17.
//  Copyright © 2017 Haijin Yan. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include "stdlib.h"
#include "sqlite3.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    //  if( argc!=3 ){
    //    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    //    return(1);
    //  }
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        fprintf(stdout, "Current working dir: %s\n", cwd);
    else
        perror("getcwd() error");
    
    // char *select = "select * from tracks where AlbumId IN (select AlbumId from albums where ArtistId = (select ArtistId from artists where name='Fretwork'))";
    char *select = "select t.*, a.* from tracks t JOIN albums a on a.albumId == t.albumId where t.AlbumId IN (select AlbumId from albums where ArtistId = (select ArtistId from artists where name='Fretwork'))";
    
    rc = sqlite3_open("/Users/haijinyan/dev/sqlitetest/sqlitetest/chinook.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }
    rc = sqlite3_exec(db, select, callback, 0, &zErrMsg);
    if( rc!=SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
    return 0;
}
