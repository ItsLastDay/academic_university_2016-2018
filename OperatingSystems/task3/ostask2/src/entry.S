.global interrupt_table
.global interrupt_table_size

.text
vector0:
    pushq $0
    pushq $0
    jmp omnihandler
vector1:
    pushq $0
    pushq $1
    jmp omnihandler
vector2:
    pushq $0
    pushq $2
    jmp omnihandler
vector3:
    pushq $0
    pushq $3
    jmp omnihandler
vector4:
    pushq $0
    pushq $4
    jmp omnihandler
vector5:
    pushq $0
    pushq $5
    jmp omnihandler
vector6:
    pushq $0
    pushq $6
    jmp omnihandler
vector7:
    pushq $0
    pushq $7
    jmp omnihandler
vector8:
    pushq $8
    jmp omnihandler
vector9:
    pushq $0
    pushq $9
    jmp omnihandler
vector10:
    pushq $10
    jmp omnihandler
vector11:
    pushq $11
    jmp omnihandler
vector12:
    pushq $12
    jmp omnihandler
vector13:
    pushq $13
    jmp omnihandler
vector14:
    pushq $14
    jmp omnihandler
vector15:
    pushq $0
    pushq $15
    jmp omnihandler
vector16:
    pushq $0
    pushq $16
    jmp omnihandler
vector17:
    pushq $17
    jmp omnihandler
vector18:
    pushq $0
    pushq $18
    jmp omnihandler
vector19:
    pushq $0
    pushq $19
    jmp omnihandler
vector20:
    pushq $0
    pushq $20
    jmp omnihandler
vector21:
    pushq $0
    pushq $21
    jmp omnihandler
vector22:
    pushq $0
    pushq $22
    jmp omnihandler
vector23:
    pushq $0
    pushq $23
    jmp omnihandler
vector24:
    pushq $0
    pushq $24
    jmp omnihandler
vector25:
    pushq $0
    pushq $25
    jmp omnihandler
vector26:
    pushq $0
    pushq $26
    jmp omnihandler
vector27:
    pushq $0
    pushq $27
    jmp omnihandler
vector28:
    pushq $0
    pushq $28
    jmp omnihandler
vector29:
    pushq $0
    pushq $29
    jmp omnihandler
vector30:
    pushq $0
    pushq $30
    jmp omnihandler
vector31:
    pushq $0
    pushq $31
    jmp omnihandler
vector32:
    pushq $0
    pushq $32
    jmp omnihandler
vector33:
    pushq $0
    pushq $33
    jmp omnihandler
vector34:
    pushq $0
    pushq $34
    jmp omnihandler
vector35:
    pushq $0
    pushq $35
    jmp omnihandler
vector36:
    pushq $0
    pushq $36
    jmp omnihandler
vector37:
    pushq $0
    pushq $37
    jmp omnihandler
vector38:
    pushq $0
    pushq $38
    jmp omnihandler
vector39:
    pushq $0
    pushq $39
    jmp omnihandler
vector40:
    pushq $0
    pushq $40
    jmp omnihandler
vector41:
    pushq $0
    pushq $41
    jmp omnihandler
vector42:
    pushq $0
    pushq $42
    jmp omnihandler
vector43:
    pushq $0
    pushq $43
    jmp omnihandler
vector44:
    pushq $0
    pushq $44
    jmp omnihandler
vector45:
    pushq $0
    pushq $45
    jmp omnihandler
vector46:
    pushq $0
    pushq $46
    jmp omnihandler
vector47:
    pushq $0
    pushq $47
    jmp omnihandler
vector48:
    pushq $0
    pushq $48
    jmp omnihandler
vector49:
    pushq $0
    pushq $49
    jmp omnihandler
vector50:
    pushq $0
    pushq $50
    jmp omnihandler
vector51:
    pushq $0
    pushq $51
    jmp omnihandler
vector52:
    pushq $0
    pushq $52
    jmp omnihandler
vector53:
    pushq $0
    pushq $53
    jmp omnihandler
vector54:
    pushq $0
    pushq $54
    jmp omnihandler
vector55:
    pushq $0
    pushq $55
    jmp omnihandler
vector56:
    pushq $0
    pushq $56
    jmp omnihandler
vector57:
    pushq $0
    pushq $57
    jmp omnihandler
vector58:
    pushq $0
    pushq $58
    jmp omnihandler
vector59:
    pushq $0
    pushq $59
    jmp omnihandler
vector60:
    pushq $0
    pushq $60
    jmp omnihandler
vector61:
    pushq $0
    pushq $61
    jmp omnihandler
vector62:
    pushq $0
    pushq $62
    jmp omnihandler
vector63:
    pushq $0
    pushq $63
    jmp omnihandler
vector64:
    pushq $0
    pushq $64
    jmp omnihandler
vector65:
    pushq $0
    pushq $65
    jmp omnihandler
vector66:
    pushq $0
    pushq $66
    jmp omnihandler
vector67:
    pushq $0
    pushq $67
    jmp omnihandler
vector68:
    pushq $0
    pushq $68
    jmp omnihandler
vector69:
    pushq $0
    pushq $69
    jmp omnihandler
vector70:
    pushq $0
    pushq $70
    jmp omnihandler
vector71:
    pushq $0
    pushq $71
    jmp omnihandler
vector72:
    pushq $0
    pushq $72
    jmp omnihandler
vector73:
    pushq $0
    pushq $73
    jmp omnihandler
vector74:
    pushq $0
    pushq $74
    jmp omnihandler
vector75:
    pushq $0
    pushq $75
    jmp omnihandler
vector76:
    pushq $0
    pushq $76
    jmp omnihandler
vector77:
    pushq $0
    pushq $77
    jmp omnihandler
vector78:
    pushq $0
    pushq $78
    jmp omnihandler
vector79:
    pushq $0
    pushq $79
    jmp omnihandler
vector80:
    pushq $0
    pushq $80
    jmp omnihandler
vector81:
    pushq $0
    pushq $81
    jmp omnihandler
vector82:
    pushq $0
    pushq $82
    jmp omnihandler
vector83:
    pushq $0
    pushq $83
    jmp omnihandler
vector84:
    pushq $0
    pushq $84
    jmp omnihandler
vector85:
    pushq $0
    pushq $85
    jmp omnihandler
vector86:
    pushq $0
    pushq $86
    jmp omnihandler
vector87:
    pushq $0
    pushq $87
    jmp omnihandler
vector88:
    pushq $0
    pushq $88
    jmp omnihandler
vector89:
    pushq $0
    pushq $89
    jmp omnihandler
vector90:
    pushq $0
    pushq $90
    jmp omnihandler
vector91:
    pushq $0
    pushq $91
    jmp omnihandler
vector92:
    pushq $0
    pushq $92
    jmp omnihandler
vector93:
    pushq $0
    pushq $93
    jmp omnihandler
vector94:
    pushq $0
    pushq $94
    jmp omnihandler
vector95:
    pushq $0
    pushq $95
    jmp omnihandler
vector96:
    pushq $0
    pushq $96
    jmp omnihandler
vector97:
    pushq $0
    pushq $97
    jmp omnihandler
vector98:
    pushq $0
    pushq $98
    jmp omnihandler
vector99:
    pushq $0
    pushq $99
    jmp omnihandler
vector100:
    pushq $0
    pushq $100
    jmp omnihandler
vector101:
    pushq $0
    pushq $101
    jmp omnihandler
vector102:
    pushq $0
    pushq $102
    jmp omnihandler
vector103:
    pushq $0
    pushq $103
    jmp omnihandler
vector104:
    pushq $0
    pushq $104
    jmp omnihandler
vector105:
    pushq $0
    pushq $105
    jmp omnihandler
vector106:
    pushq $0
    pushq $106
    jmp omnihandler
vector107:
    pushq $0
    pushq $107
    jmp omnihandler
vector108:
    pushq $0
    pushq $108
    jmp omnihandler
vector109:
    pushq $0
    pushq $109
    jmp omnihandler
vector110:
    pushq $0
    pushq $110
    jmp omnihandler
vector111:
    pushq $0
    pushq $111
    jmp omnihandler
vector112:
    pushq $0
    pushq $112
    jmp omnihandler
vector113:
    pushq $0
    pushq $113
    jmp omnihandler
vector114:
    pushq $0
    pushq $114
    jmp omnihandler
vector115:
    pushq $0
    pushq $115
    jmp omnihandler
vector116:
    pushq $0
    pushq $116
    jmp omnihandler
vector117:
    pushq $0
    pushq $117
    jmp omnihandler
vector118:
    pushq $0
    pushq $118
    jmp omnihandler
vector119:
    pushq $0
    pushq $119
    jmp omnihandler
vector120:
    pushq $0
    pushq $120
    jmp omnihandler
vector121:
    pushq $0
    pushq $121
    jmp omnihandler
vector122:
    pushq $0
    pushq $122
    jmp omnihandler
vector123:
    pushq $0
    pushq $123
    jmp omnihandler
vector124:
    pushq $0
    pushq $124
    jmp omnihandler
vector125:
    pushq $0
    pushq $125
    jmp omnihandler
vector126:
    pushq $0
    pushq $126
    jmp omnihandler
vector127:
    pushq $0
    pushq $127
    jmp omnihandler
vector128:
    pushq $0
    pushq $128
    jmp omnihandler
vector129:
    pushq $0
    pushq $129
    jmp omnihandler
vector130:
    pushq $0
    pushq $130
    jmp omnihandler
vector131:
    pushq $0
    pushq $131
    jmp omnihandler
vector132:
    pushq $0
    pushq $132
    jmp omnihandler
vector133:
    pushq $0
    pushq $133
    jmp omnihandler
vector134:
    pushq $0
    pushq $134
    jmp omnihandler
vector135:
    pushq $0
    pushq $135
    jmp omnihandler
vector136:
    pushq $0
    pushq $136
    jmp omnihandler
vector137:
    pushq $0
    pushq $137
    jmp omnihandler
vector138:
    pushq $0
    pushq $138
    jmp omnihandler
vector139:
    pushq $0
    pushq $139
    jmp omnihandler
vector140:
    pushq $0
    pushq $140
    jmp omnihandler
vector141:
    pushq $0
    pushq $141
    jmp omnihandler
vector142:
    pushq $0
    pushq $142
    jmp omnihandler
vector143:
    pushq $0
    pushq $143
    jmp omnihandler
vector144:
    pushq $0
    pushq $144
    jmp omnihandler
vector145:
    pushq $0
    pushq $145
    jmp omnihandler
vector146:
    pushq $0
    pushq $146
    jmp omnihandler
vector147:
    pushq $0
    pushq $147
    jmp omnihandler
vector148:
    pushq $0
    pushq $148
    jmp omnihandler
vector149:
    pushq $0
    pushq $149
    jmp omnihandler
vector150:
    pushq $0
    pushq $150
    jmp omnihandler
vector151:
    pushq $0
    pushq $151
    jmp omnihandler
vector152:
    pushq $0
    pushq $152
    jmp omnihandler
vector153:
    pushq $0
    pushq $153
    jmp omnihandler
vector154:
    pushq $0
    pushq $154
    jmp omnihandler
vector155:
    pushq $0
    pushq $155
    jmp omnihandler
vector156:
    pushq $0
    pushq $156
    jmp omnihandler
vector157:
    pushq $0
    pushq $157
    jmp omnihandler
vector158:
    pushq $0
    pushq $158
    jmp omnihandler
vector159:
    pushq $0
    pushq $159
    jmp omnihandler
vector160:
    pushq $0
    pushq $160
    jmp omnihandler
vector161:
    pushq $0
    pushq $161
    jmp omnihandler
vector162:
    pushq $0
    pushq $162
    jmp omnihandler
vector163:
    pushq $0
    pushq $163
    jmp omnihandler
vector164:
    pushq $0
    pushq $164
    jmp omnihandler
vector165:
    pushq $0
    pushq $165
    jmp omnihandler
vector166:
    pushq $0
    pushq $166
    jmp omnihandler
vector167:
    pushq $0
    pushq $167
    jmp omnihandler
vector168:
    pushq $0
    pushq $168
    jmp omnihandler
vector169:
    pushq $0
    pushq $169
    jmp omnihandler
vector170:
    pushq $0
    pushq $170
    jmp omnihandler
vector171:
    pushq $0
    pushq $171
    jmp omnihandler
vector172:
    pushq $0
    pushq $172
    jmp omnihandler
vector173:
    pushq $0
    pushq $173
    jmp omnihandler
vector174:
    pushq $0
    pushq $174
    jmp omnihandler
vector175:
    pushq $0
    pushq $175
    jmp omnihandler
vector176:
    pushq $0
    pushq $176
    jmp omnihandler
vector177:
    pushq $0
    pushq $177
    jmp omnihandler
vector178:
    pushq $0
    pushq $178
    jmp omnihandler
vector179:
    pushq $0
    pushq $179
    jmp omnihandler
vector180:
    pushq $0
    pushq $180
    jmp omnihandler
vector181:
    pushq $0
    pushq $181
    jmp omnihandler
vector182:
    pushq $0
    pushq $182
    jmp omnihandler
vector183:
    pushq $0
    pushq $183
    jmp omnihandler
vector184:
    pushq $0
    pushq $184
    jmp omnihandler
vector185:
    pushq $0
    pushq $185
    jmp omnihandler
vector186:
    pushq $0
    pushq $186
    jmp omnihandler
vector187:
    pushq $0
    pushq $187
    jmp omnihandler
vector188:
    pushq $0
    pushq $188
    jmp omnihandler
vector189:
    pushq $0
    pushq $189
    jmp omnihandler
vector190:
    pushq $0
    pushq $190
    jmp omnihandler
vector191:
    pushq $0
    pushq $191
    jmp omnihandler
vector192:
    pushq $0
    pushq $192
    jmp omnihandler
vector193:
    pushq $0
    pushq $193
    jmp omnihandler
vector194:
    pushq $0
    pushq $194
    jmp omnihandler
vector195:
    pushq $0
    pushq $195
    jmp omnihandler
vector196:
    pushq $0
    pushq $196
    jmp omnihandler
vector197:
    pushq $0
    pushq $197
    jmp omnihandler
vector198:
    pushq $0
    pushq $198
    jmp omnihandler
vector199:
    pushq $0
    pushq $199
    jmp omnihandler
vector200:
    pushq $0
    pushq $200
    jmp omnihandler
vector201:
    pushq $0
    pushq $201
    jmp omnihandler
vector202:
    pushq $0
    pushq $202
    jmp omnihandler
vector203:
    pushq $0
    pushq $203
    jmp omnihandler
vector204:
    pushq $0
    pushq $204
    jmp omnihandler
vector205:
    pushq $0
    pushq $205
    jmp omnihandler
vector206:
    pushq $0
    pushq $206
    jmp omnihandler
vector207:
    pushq $0
    pushq $207
    jmp omnihandler
vector208:
    pushq $0
    pushq $208
    jmp omnihandler
vector209:
    pushq $0
    pushq $209
    jmp omnihandler
vector210:
    pushq $0
    pushq $210
    jmp omnihandler
vector211:
    pushq $0
    pushq $211
    jmp omnihandler
vector212:
    pushq $0
    pushq $212
    jmp omnihandler
vector213:
    pushq $0
    pushq $213
    jmp omnihandler
vector214:
    pushq $0
    pushq $214
    jmp omnihandler
vector215:
    pushq $0
    pushq $215
    jmp omnihandler
vector216:
    pushq $0
    pushq $216
    jmp omnihandler
vector217:
    pushq $0
    pushq $217
    jmp omnihandler
vector218:
    pushq $0
    pushq $218
    jmp omnihandler
vector219:
    pushq $0
    pushq $219
    jmp omnihandler
vector220:
    pushq $0
    pushq $220
    jmp omnihandler
vector221:
    pushq $0
    pushq $221
    jmp omnihandler
vector222:
    pushq $0
    pushq $222
    jmp omnihandler
vector223:
    pushq $0
    pushq $223
    jmp omnihandler
vector224:
    pushq $0
    pushq $224
    jmp omnihandler
vector225:
    pushq $0
    pushq $225
    jmp omnihandler
vector226:
    pushq $0
    pushq $226
    jmp omnihandler
vector227:
    pushq $0
    pushq $227
    jmp omnihandler
vector228:
    pushq $0
    pushq $228
    jmp omnihandler
vector229:
    pushq $0
    pushq $229
    jmp omnihandler
vector230:
    pushq $0
    pushq $230
    jmp omnihandler
vector231:
    pushq $0
    pushq $231
    jmp omnihandler
vector232:
    pushq $0
    pushq $232
    jmp omnihandler
vector233:
    pushq $0
    pushq $233
    jmp omnihandler
vector234:
    pushq $0
    pushq $234
    jmp omnihandler
vector235:
    pushq $0
    pushq $235
    jmp omnihandler
vector236:
    pushq $0
    pushq $236
    jmp omnihandler
vector237:
    pushq $0
    pushq $237
    jmp omnihandler
vector238:
    pushq $0
    pushq $238
    jmp omnihandler
vector239:
    pushq $0
    pushq $239
    jmp omnihandler
vector240:
    pushq $0
    pushq $240
    jmp omnihandler
vector241:
    pushq $0
    pushq $241
    jmp omnihandler
vector242:
    pushq $0
    pushq $242
    jmp omnihandler
vector243:
    pushq $0
    pushq $243
    jmp omnihandler
vector244:
    pushq $0
    pushq $244
    jmp omnihandler
vector245:
    pushq $0
    pushq $245
    jmp omnihandler
vector246:
    pushq $0
    pushq $246
    jmp omnihandler
vector247:
    pushq $0
    pushq $247
    jmp omnihandler
vector248:
    pushq $0
    pushq $248
    jmp omnihandler
vector249:
    pushq $0
    pushq $249
    jmp omnihandler
vector250:
    pushq $0
    pushq $250
    jmp omnihandler
vector251:
    pushq $0
    pushq $251
    jmp omnihandler
vector252:
    pushq $0
    pushq $252
    jmp omnihandler
vector253:
    pushq $0
    pushq $253
    jmp omnihandler
vector254:
    pushq $0
    pushq $254
    jmp omnihandler
vector255:
    pushq $0
    pushq $255
    jmp omnihandler

omnihandler:
    // Do not save %rbx, %rbp, and %r12–%r15,
    // according to p.25 at http://mit.spbau.ru/sewiki/images/1/13/Asm64-handout.pdf
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rdi
    pushq %rsi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11 

    movq %rsp, %rdi
    // Page 15 from http://mit.spbau.ru/sewiki/images/7/75/Abi_sysV_amd64.pdf:
    // "The direction flag in the %eflags register must be clear on function entry, and on function return."
    cld
    call interrupt_route
    // C-functions do `cld` on their own. Moreover, we restore 
    // previous rflags when doing `iretq` below.

    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rsi
    popq %rdi
    popq %rdx
    popq %rcx
    popq %rax

    addq $16, %rsp  // Error code, vector.

    iretq

    

interrupt_table:
    .quad vector0
    .quad vector1
    .quad vector2
    .quad vector3
    .quad vector4
    .quad vector5
    .quad vector6
    .quad vector7
    .quad vector8
    .quad vector9
    .quad vector10
    .quad vector11
    .quad vector12
    .quad vector13
    .quad vector14
    .quad vector15
    .quad vector16
    .quad vector17
    .quad vector18
    .quad vector19
    .quad vector20
    .quad vector21
    .quad vector22
    .quad vector23
    .quad vector24
    .quad vector25
    .quad vector26
    .quad vector27
    .quad vector28
    .quad vector29
    .quad vector30
    .quad vector31
    .quad vector32
    .quad vector33
    .quad vector34
    .quad vector35
    .quad vector36
    .quad vector37
    .quad vector38
    .quad vector39
    .quad vector40
    .quad vector41
    .quad vector42
    .quad vector43
    .quad vector44
    .quad vector45
    .quad vector46
    .quad vector47
    .quad vector48
    .quad vector49
    .quad vector50
    .quad vector51
    .quad vector52
    .quad vector53
    .quad vector54
    .quad vector55
    .quad vector56
    .quad vector57
    .quad vector58
    .quad vector59
    .quad vector60
    .quad vector61
    .quad vector62
    .quad vector63
    .quad vector64
    .quad vector65
    .quad vector66
    .quad vector67
    .quad vector68
    .quad vector69
    .quad vector70
    .quad vector71
    .quad vector72
    .quad vector73
    .quad vector74
    .quad vector75
    .quad vector76
    .quad vector77
    .quad vector78
    .quad vector79
    .quad vector80
    .quad vector81
    .quad vector82
    .quad vector83
    .quad vector84
    .quad vector85
    .quad vector86
    .quad vector87
    .quad vector88
    .quad vector89
    .quad vector90
    .quad vector91
    .quad vector92
    .quad vector93
    .quad vector94
    .quad vector95
    .quad vector96
    .quad vector97
    .quad vector98
    .quad vector99
    .quad vector100
    .quad vector101
    .quad vector102
    .quad vector103
    .quad vector104
    .quad vector105
    .quad vector106
    .quad vector107
    .quad vector108
    .quad vector109
    .quad vector110
    .quad vector111
    .quad vector112
    .quad vector113
    .quad vector114
    .quad vector115
    .quad vector116
    .quad vector117
    .quad vector118
    .quad vector119
    .quad vector120
    .quad vector121
    .quad vector122
    .quad vector123
    .quad vector124
    .quad vector125
    .quad vector126
    .quad vector127
    .quad vector128
    .quad vector129
    .quad vector130
    .quad vector131
    .quad vector132
    .quad vector133
    .quad vector134
    .quad vector135
    .quad vector136
    .quad vector137
    .quad vector138
    .quad vector139
    .quad vector140
    .quad vector141
    .quad vector142
    .quad vector143
    .quad vector144
    .quad vector145
    .quad vector146
    .quad vector147
    .quad vector148
    .quad vector149
    .quad vector150
    .quad vector151
    .quad vector152
    .quad vector153
    .quad vector154
    .quad vector155
    .quad vector156
    .quad vector157
    .quad vector158
    .quad vector159
    .quad vector160
    .quad vector161
    .quad vector162
    .quad vector163
    .quad vector164
    .quad vector165
    .quad vector166
    .quad vector167
    .quad vector168
    .quad vector169
    .quad vector170
    .quad vector171
    .quad vector172
    .quad vector173
    .quad vector174
    .quad vector175
    .quad vector176
    .quad vector177
    .quad vector178
    .quad vector179
    .quad vector180
    .quad vector181
    .quad vector182
    .quad vector183
    .quad vector184
    .quad vector185
    .quad vector186
    .quad vector187
    .quad vector188
    .quad vector189
    .quad vector190
    .quad vector191
    .quad vector192
    .quad vector193
    .quad vector194
    .quad vector195
    .quad vector196
    .quad vector197
    .quad vector198
    .quad vector199
    .quad vector200
    .quad vector201
    .quad vector202
    .quad vector203
    .quad vector204
    .quad vector205
    .quad vector206
    .quad vector207
    .quad vector208
    .quad vector209
    .quad vector210
    .quad vector211
    .quad vector212
    .quad vector213
    .quad vector214
    .quad vector215
    .quad vector216
    .quad vector217
    .quad vector218
    .quad vector219
    .quad vector220
    .quad vector221
    .quad vector222
    .quad vector223
    .quad vector224
    .quad vector225
    .quad vector226
    .quad vector227
    .quad vector228
    .quad vector229
    .quad vector230
    .quad vector231
    .quad vector232
    .quad vector233
    .quad vector234
    .quad vector235
    .quad vector236
    .quad vector237
    .quad vector238
    .quad vector239
    .quad vector240
    .quad vector241
    .quad vector242
    .quad vector243
    .quad vector244
    .quad vector245
    .quad vector246
    .quad vector247
    .quad vector248
    .quad vector249
    .quad vector250
    .quad vector251
    .quad vector252
    .quad vector253
    .quad vector254
    .quad vector255

interrupt_table_size:
    .word (interrupt_table_size - interrupt_table) / 8
