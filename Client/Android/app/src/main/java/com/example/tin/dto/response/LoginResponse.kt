package com.example.tin.dto.response

import com.fasterxml.jackson.annotation.JsonIgnoreProperties

@JsonIgnoreProperties("type")
class LoginResponse {
    var role = 0
    var status = -1
}