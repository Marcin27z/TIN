package com.example.tin.dto.request

class CancelConsultationRequest(
    val id: String,
    val login: String,
    val type: String = "CancelConsultationRequest"
) {
}