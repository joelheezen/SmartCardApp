package com.sc.smartcard.data.database

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.Query
import com.sc.smartcard.data.model.BarcodeEntity

@Dao
interface BarcodeDao {
    @Insert
    suspend fun insertBarcode(barcode: BarcodeEntity)

    @Query("SELECT * FROM barcodes")
    suspend fun getAllBarcodes(): List<BarcodeEntity>
}