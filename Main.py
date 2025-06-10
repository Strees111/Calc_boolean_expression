import sys
sys.path.insert(0, "./build")

import boolcalc
import asyncio
from telebot.async_telebot import AsyncTeleBot
from telebot import types
import io
from PIL import Image, ImageDraw, ImageFont
import csv

bot = AsyncTeleBot("7908905794:AAEa1uuoorYLkkN14mZ1E8K7GWh5qWRX5t4")

user_states = {}
STATE_WAITING_FOR_ADD = 'add_expression'
STATE_WAITING_FOR_DELETE = 'delete_expression'



@bot.message_handler(commands=['start'])
async def handle_start(message):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    btn1 = types.KeyboardButton('Таблица')
    btn2 = types.KeyboardButton('Полином Жегалкина')
    btn3 = types.KeyboardButton('Проверка на полноту')
    markup.row(btn1, btn2, btn3)
    btn4 = types.KeyboardButton('Справка')
    btn5 = types.KeyboardButton('Очистка системы')
    btn6 = types.KeyboardButton('Отобразить систему')
    btn7 = types.KeyboardButton('Добавить или удалить выражение')
    markup.row(btn4,btn5,btn6)
    markup.row(btn7)
    welcome_message = (
        "Привет! Я бот для работы с булевыми выражениями.\n\n"
        "<b>Переменные в выражении:</b>\n"
        "Используйте формат <code>xN</code>, где N — натуральное число от 0 до 9 (например, <code>x1</code>, <code>x2</code>, <code>x3</code>, ...).\n"
        "Переменные могут принимать значения 0 или 1.\n\n"
        "<b>Поддерживаемые операции:</b>\n"
        "<code>~</code>   отрицание\n"
        "<code>&</code>   конъюнкция\n"
        "<code>v</code>   дизъюнкция\n"
        "<code>+</code>   сложение по модулю 2\n"
        "<code>&gt</code>   импликация (x → y)\n"
        "<code>&lt</code>   обратная импликация (x ← y)\n"
        "<code>=</code>   эквиваленция (x ≡ y)\n"
        "<code>|</code>   штрих Шеффера (x | y)\n"
        "<code>^</code>   стрелка Пирса (x ↓ y)\n\n"
        "<b>Примечание!!!!!!:</b> Если в выражений различных переменных больше 6, то таблица истинности будет выводиться ввиде CSV файла."
    )
    await bot.send_message(message.chat.id, welcome_message, reply_markup=markup, parse_mode='HTML')

def table_to_csv(table_text):
    lines = table_text.strip().split('\n')
    csv_buffer = io.StringIO()
    csv_writer = csv.writer(csv_buffer)
    for line in lines:
        row = [cell for cell in line.split(' ') if cell]
        csv_writer.writerow(row)
    byte_buffer = io.BytesIO()
    byte_buffer.write(csv_buffer.getvalue().encode('utf-8'))
    byte_buffer.seek(0)
    return byte_buffer

def text_to_image(text, width=None, height=None, bg_color=(255, 255, 255)):
    lines_count = text.count('\n') + 1
    try:
        max_line_len = max(len(line) for line in text.split('\n'))
    except ValueError:
        max_line_len = 10
    if width is None:
        width = max(max_line_len * 10, 250)
    if height is None:
        height = max(150, lines_count * 22)
    image = Image.new("RGB", (width, height), bg_color)
    draw = ImageDraw.Draw(image)
    try:
        font = ImageFont.truetype("fonts/NotoSansMono.ttf", 16)
    except IOError:
        font = ImageFont.load_default(16)
    draw.text((20, 20), text, fill=(0, 0, 0), font=font)
    bio = io.BytesIO()
    bio.name = 'result.png'
    image.save(bio, 'PNG')
    bio.seek(0)
    return bio


@bot.message_handler(content_types=['text'])
async def handle_text_messages(message):
    user_id = message.chat.id
    
    state = user_states.pop(user_id, None)

    if state == STATE_WAITING_FOR_ADD:
        await add_expression(message)
        return 
    elif state == STATE_WAITING_FOR_DELETE:
        await delete_expression(message)
        return
    
    try:
        if message.text == 'Таблица':
            result_table = await asyncio.to_thread(boolcalc.process_expression, "-table", message.chat.id)
            result_table = result_table.split('\n')
            
            await bot.send_message(message.chat.id, f"Вектор функция: \n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_table) if expr])}")
            
            table_data = await asyncio.to_thread(boolcalc.process_expression, "-FullTable", message.chat.id)
            tables = table_data.split('=')
            
            j = 0
            for i, expr in enumerate(tables):
                if not expr.strip():
                    continue
                expression_str = await asyncio.to_thread(boolcalc.get_expression, message.chat.id, j)
                if len(result_table[j]) > 64:
                    csv_file = table_to_csv(expr)
                    await bot.send_document(message.chat.id, csv_file, caption=f"Таблица истинности для {expression_str} в формате CSV", visible_file_name=f"{i+1}truth_table.csv")
                else:
                    text = f"{expr}"
                    img_bio = await asyncio.to_thread(text_to_image, text)
                    await bot.send_photo(message.chat.id, photo=img_bio, caption=f'Таблица истинности для\n{expression_str}')
                j += 1
        elif message.text == 'Полином Жегалкина':
            result_zh = await asyncio.to_thread(boolcalc.process_expression, "-zh", message.chat.id)
            result_zh = result_zh.split('\n')
            await bot.send_message(message.chat.id, f"Жегалкин:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_zh) if expr])}")
        elif message.text == 'Проверка на полноту':
            is_full = await asyncio.to_thread(boolcalc.process_expression, "-isfull", message.chat.id)
            await bot.send_message(message.chat.id, f"Полнота системы: {is_full}")
        elif message.text == 'Справка':
            await help(message)
        elif message.text == 'Очистка системы':
            await clear(message)
            await bot.send_message(message.chat.id, "<b>Список очищен.</b>\n\n", parse_mode='HTML')
        elif message.text == 'Отобразить систему':
            await open_lines(message)
        elif message.text == 'Добавить или удалить выражение':
            await get_expression(message)
    except Exception as e:
        error_message = f"Произошла ошибка: {e}"
        await bot.send_message(message.chat.id, error_message)
        print(error_message)

async def help(message):
    help_text = (
        "<b>Переменные в выражении:</b>\n"
        "Используйте формат <code>xN</code>, где N — натуральное число от 0 до 9 (например, <code>x1</code>, <code>x2</code>, <code>x3</code>, ...).\n"
        "Переменные могут принимать значения 0 или 1.\n\n"
        "<b>Поддерживаемые операции:</b>\n"
        "<code>~</code>   отрицание\n"
        "<code>&</code>   конъюнкция\n"
        "<code>v</code>   дизъюнкция\n"
        "<code>+</code>   сложение по модулю 2\n"
        "<code>&gt</code>   импликация (x → y)\n"
        "<code>&lt</code>   обратная импликация (x ← y)\n"
        "<code>=</code>   эквиваленция (x ≡ y)\n"
        "<code>|</code>   штрих Шеффера (x | y)\n"
        "<code>^</code>   стрелка Пирса (x ↓ y)\n\n"
        "<b>Действия с выражениями (через кнопки):</b>\n"
        "<b>Таблица</b>: Построить таблицу истинности для всех добавленных выражений.\n"
        "<b>Полином Жегалкина</b>: Построить полином Жегалкина для всех добавленных выражений.\n"
        "<b>Проверка на полноту</b>: Проверить систему добавленных функций на полноту.\n\n"
        "<b>Примечание!!!!!!:</b> Если в выражений различных переменных больше 6, то таблица истинности будет выводиться ввиде CSV файла."
    )
    await bot.send_message(message.chat.id, help_text, parse_mode='HTML')


async def clear(message):
    await asyncio.to_thread(boolcalc.clear_expressions, message.chat.id)

async def open_lines(message):
    current_user_lines = await asyncio.to_thread(boolcalc.print_expressions, message.chat.id)
    if current_user_lines:
        await bot.send_message(message.chat.id, f"<b>Система:</b>\n{current_user_lines}", parse_mode='HTML')
    else:
        await bot.send_message(message.chat.id, "Список выражений пуст.")

async def get_expression(message):
    markup = types.InlineKeyboardMarkup()
    btn1 = types.InlineKeyboardButton('Добавить выражение', callback_data='Add')
    btn2 = types.InlineKeyboardButton('Удалить выражение', callback_data='Delete')
    markup.row(btn1, btn2)
    await bot.send_message(message.chat.id, 'Выберите действие:', reply_markup=markup)

@bot.callback_query_handler(func=lambda callback: True)
async def callback_message(callback):
    await bot.delete_message(callback.message.chat.id, callback.message.message_id)
    
    
    if callback.data == 'Add':
        await bot.send_message(callback.message.chat.id, 'Введите булевое выражение:')
        user_states[callback.message.chat.id] = STATE_WAITING_FOR_ADD
        
    elif callback.data == 'Delete':
        is_empty = await asyncio.to_thread(boolcalc.empty_expressions, callback.message.chat.id)
        if not is_empty:
            expressions = await asyncio.to_thread(boolcalc.print_expressions, callback.message.chat.id)
            await bot.send_message(callback.message.chat.id, f"Текущие выражения:\n{expressions}\n\nВведите номер выражения для удаления:")
            user_states[callback.message.chat.id] = STATE_WAITING_FOR_DELETE
        else:
            await bot.send_message(callback.message.chat.id, "Список выражений пуст.")
            

async def add_expression(message):
    try:
        await asyncio.to_thread(boolcalc.add_expression, message.chat.id, message.text)
        await bot.send_message(message.chat.id, f"Выражение '{message.text}' добавлено в список.")
    except Exception as e:
        error_message = f"Не удалось добавить выражение. Ошибка: {e}"
        await bot.send_message(message.chat.id, error_message)

async def delete_expression(message):
    try:
        index = int(message.text) - 1
        deleted = await asyncio.to_thread(boolcalc.delete_expression, message.chat.id, index)
        await bot.send_message(message.chat.id, f"Выражение '{deleted}' удалено.")
    except (ValueError, IndexError) as e:
        await bot.send_message(message.chat.id, f"Не удалось удалить выражение. Введите корректный номер. Ошибка: {e}")

async def main():
    print("Бот запускается...")
    await bot.polling(non_stop=True)

if __name__ == '__main__':
    asyncio.run(main())