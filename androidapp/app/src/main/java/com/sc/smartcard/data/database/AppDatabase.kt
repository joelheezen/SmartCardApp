package com.sc.smartcard.data.database

import androidx.room.Database
import androidx.room.RoomDatabase
import com.sc.smartcard.data.model.BarcodeEntity

@Database(entities = [BarcodeEntity::class], version = 1)
abstract class AppDatabase : RoomDatabase(){

    abstract fun barcodeDao(): BarcodeDao
}