from build import boolcalc
import asyncio
from telebot.async_telebot import AsyncTeleBot
from telebot import types
import io
from PIL import Image, ImageDraw, ImageFont
import csv
from dotenv import load_dotenv
import os

load_dotenv()  # loads variables from .env into os.environ

api_key = os.getenv("TELEGRAM_API_KEY")

if not api_key:
    raise ValueError("No TELEGRAM_API_KEY set in environment")

bot = AsyncTeleBot(api_key)

user_states = {}
STATE_WAITING_FOR_ADD = 'add_expression'
STATE_WAITING_FOR_DELETE = 'delete_expression'

def get_main_keyboard():
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True, is_persistent=True)
    btn1 = types.KeyboardButton('Таблица')
    btn2 = types.KeyboardButton('Полином Жегалкина')
    btn3 = types.KeyboardButton('Проверка на полноту')
    btn8 = types.KeyboardButton('СКНФ')
    btn9 = types.KeyboardButton('СДНФ')
    btn10 = types.KeyboardButton('Минимизируемый ДНФ')
    btn11 = types.KeyboardButton('Минимизируемый КНФ')
    markup.row(btn1, btn2, btn3)
    markup.row(btn8, btn9, btn10, btn11)
    btn4 = types.KeyboardButton('Справка')
    btn5 = types.KeyboardButton('Очистка системы')
    btn6 = types.KeyboardButton('Отобразить систему')
    btn7 = types.KeyboardButton('Добавить или удалить выражение')
    markup.row(btn4,btn5,btn6)
    markup.row(btn7)
    return markup

@bot.message_handler(commands=['start'])
async def handle_start(message):
    markup = get_main_keyboard()
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
        "<b>Проверка на полноту</b>: Проверить систему добавленных функций на полноту. (Если вывело все 5 минусов, то система полная)\n\n"
        "<b>СДНФ</b>: Построить СДНФ для всех добавленных выражений.\n\n"
        "<b>СКНФ</b>: Построить СДНФ для всех добавленных выражений.\n\n"
        "<b>Минимизируемый ДНФ</b>: Построить Минимизируемый ДНФ для всех добавленных выражений.\n\n"
        "<b>Минимизируемый КНФ</b>: Построить Минимизируемый КНФ для всех добавленных выражений.\n\n"
        "<b>Примечание №1!!!!!!:</b> Если в выражений различных переменных больше 6, то таблица истинности будет выводиться ввиде CSV файла.\n\n"
        "<b>Примечание №2!!!!!!:</b> Данный бот может выводить неточную информацию. Советую всё проверять самим, особенно минимизацию.\n\n"
        "<b>Примечание №3!!!!!!:</b> Ограничение в 4000 символов в сообщении.\n\n"
        "<b>Примечание №4!!!!!!:</b> Если найдёте ошибку пишите в личку @Crazyfox4.\n\n"
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
    markup = get_main_keyboard()
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
            
            await bot.send_message(message.chat.id, f"Вектор функция: \n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_table) if expr])}", reply_markup=markup)
            
            table_data = await asyncio.to_thread(boolcalc.process_expression, "-FullTable", message.chat.id)
            tables = table_data.split('=')
            
            j = 0
            for i, expr in enumerate(tables):
                if not expr.strip():
                    continue
                expression_str = await asyncio.to_thread(boolcalc.get_expression, message.chat.id, j)
                if len(result_table[j]) > 64:
                    csv_file = table_to_csv(expr)
                    await bot.send_document(message.chat.id, csv_file, caption=f"Таблица истинности для {expression_str} в формате CSV", visible_file_name=f"{i+1}truth_table.csv", reply_markup=markup)
                else:
                    text = f"{expr}"
                    img_bio = await asyncio.to_thread(text_to_image, text)
                    await bot.send_photo(message.chat.id, photo=img_bio, caption=f'Таблица истинности для\n{expression_str}', reply_markup=markup)
                j += 1
        elif message.text == 'Полином Жегалкина':
            result_zh = await asyncio.to_thread(boolcalc.process_expression, "-zh", message.chat.id)
            result_zh = result_zh.split('\n')
            await bot.send_message(message.chat.id, f"Жегалкин:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_zh) if expr])}", reply_markup=markup)
        elif message.text == 'Проверка на полноту':
            is_full = await asyncio.to_thread(boolcalc.process_expression, "-isfull", message.chat.id)
            await bot.send_message(message.chat.id, f"Проверка на полноту системы: \n{is_full}",reply_markup=markup)
        elif message.text == 'СКНФ':
            sknf = await asyncio.to_thread(boolcalc.process_expression, "-sknf", message.chat.id)
            sknf = sknf.split('\n')
            await bot.send_message(message.chat.id, f"СКНФ:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(sknf) if expr])}", reply_markup=markup)
        elif message.text == 'СДНФ':
            sdnf = await asyncio.to_thread(boolcalc.process_expression, "-sdnf", message.chat.id)
            sdnf = sdnf.split('\n')
            await bot.send_message(message.chat.id, f"СДНФ:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(sdnf) if expr])}", reply_markup=markup)
        elif message.text == 'Минимизируемый ДНФ':
            mdnf = await asyncio.to_thread(boolcalc.process_expression, "-MinimizeDNF", message.chat.id)
            mdnf = mdnf.split('\n')
            await bot.send_message(message.chat.id, f"Минимизируемый ДНФ:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(mdnf) if expr])}", reply_markup=markup)
        elif message.text == 'Минимизируемый КНФ':
            mcnf = await asyncio.to_thread(boolcalc.process_expression, "-MinimizeCNF", message.chat.id)
            mcnf = mcnf.split('\n')
            await bot.send_message(message.chat.id, f"Минимизируемый КНФ:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(mcnf) if expr])}", reply_markup=markup)
        elif message.text == 'Справка':
            await help(message)
        elif message.text == 'Очистка системы':
            await clear(message)
            await bot.send_message(message.chat.id, "<b>Список очищен.</b>\n\n", parse_mode='HTML', reply_markup=markup)
        elif message.text == 'Отобразить систему':
            await open_lines(message)
        elif message.text == 'Добавить или удалить выражение':
            await get_expression(message)
        else:
            await bot.send_message(message.chat.id, "Выберите действие с помощью кнопок ниже или используйте /start для справки:", reply_markup=markup)    
    except Exception as e:
        error_message = f"Произошла ошибка: {e}"
        await bot.send_message(message.chat.id, error_message, reply_markup=markup)
        print(error_message)

async def help(message):
    markup = get_main_keyboard()
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
        "<b>Проверка на полноту</b>: Проверить систему добавленных функций на полноту. (Если вывело все 5 минусов, то система полная)\n\n"
        "<b>СДНФ</b>: Построить СДНФ для всех добавленных выражений.\n\n"
        "<b>СКНФ</b>: Построить СДНФ для всех добавленных выражений.\n\n"
        "<b>Минимизируемый ДНФ</b>: Построить Минимизируемый ДНФ для всех добавленных выражений.\n\n"
        "<b>Минимизируемый КНФ</b>: Построить Минимизируемый КНФ для всех добавленных выражений.\n\n"
        "<b>Примечание №1!!!!!!:</b> Если в выражений различных переменных больше 6, то таблица истинности будет выводиться ввиде CSV файла.\n\n"
        "<b>Примечание №2!!!!!!:</b> Данный бот может выводить неточную информацию. Советую всё проверять самим, особенно минимизацию.\n\n"
        "<b>Примечание №3!!!!!!:</b> Ограничение в 4000 символов в сообщении.\n\n"
        "<b>Примечание №4!!!!!!:</b> Если найдёте ошибку пишите в личку @Crazyfox4.\n\n"
    )
    await bot.send_message(message.chat.id, help_text, parse_mode='HTML', reply_markup=markup)


async def clear(message):
    await asyncio.to_thread(boolcalc.clear_expressions, message.chat.id)

async def open_lines(message):
    markup = get_main_keyboard()
    current_user_lines = await asyncio.to_thread(boolcalc.print_expressions, message.chat.id)
    if current_user_lines:
        await bot.send_message(message.chat.id, f"<b>Система:</b>\n{current_user_lines}", parse_mode='HTML', reply_markup=markup)
    else:
        await bot.send_message(message.chat.id, "Список выражений пуст.", reply_markup=markup)

async def get_expression(message):
    markup = types.InlineKeyboardMarkup()
    btn1 = types.InlineKeyboardButton('Добавить выражение', callback_data='Add')
    btn2 = types.InlineKeyboardButton('Удалить выражение', callback_data='Delete')
    markup.row(btn1, btn2)
    await bot.send_message(message.chat.id, 'Выберите действие:', reply_markup=markup)

@bot.callback_query_handler(func=lambda callback: True)
async def callback_message(callback):
    markup = get_main_keyboard()
    await bot.delete_message(callback.message.chat.id, callback.message.message_id)
    
    
    if callback.data == 'Add':
        await bot.send_message(callback.message.chat.id, 'Введите булевое выражение:',reply_markup=markup)
        user_states[callback.message.chat.id] = STATE_WAITING_FOR_ADD
        
    elif callback.data == 'Delete':
        is_empty = await asyncio.to_thread(boolcalc.empty_expressions, callback.message.chat.id)
        if not is_empty:
            expressions = await asyncio.to_thread(boolcalc.print_expressions, callback.message.chat.id)
            await bot.send_message(callback.message.chat.id, f"Текущие выражения:\n{expressions}\n\nВведите номер выражения для удаления:",reply_markup=markup)
            user_states[callback.message.chat.id] = STATE_WAITING_FOR_DELETE
        else:
            await bot.send_message(callback.message.chat.id, "Список выражений пуст.",reply_markup=markup)
            

async def add_expression(message):
    markup = get_main_keyboard()
    try:
        await asyncio.to_thread(boolcalc.add_expression, message.chat.id, message.text)
        await bot.send_message(message.chat.id, f"Выражение '{message.text}' добавлено в список.", reply_markup=markup)
    except Exception as e:
        error_message = f"Не удалось добавить выражение. Ошибка: {e}"
        await bot.send_message(message.chat.id, error_message, reply_markup=markup)

async def delete_expression(message):
    markup = get_main_keyboard()
    try:
        index = int(message.text) - 1
        deleted = await asyncio.to_thread(boolcalc.delete_expression, message.chat.id, index)
        await bot.send_message(message.chat.id, f"Выражение '{deleted}' удалено.", reply_markup=markup)
    except (ValueError, IndexError) as e:
        await bot.send_message(message.chat.id, f"Не удалось удалить выражение. Введите корректный номер. Ошибка: {e}", reply_markup=markup)

async def main():
    print("Бот запускается...")
    await bot.polling(non_stop=True)

if __name__ == '__main__':
    asyncio.run(main())